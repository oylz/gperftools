#include <stdio.h>
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>
#include <string.h>

static const int LEN = 8;
std::string get_symbol(const std::string &lib, uint64_t mpc);

static uint64_t read8(unsigned char *buf, int *pos){
    uint64_t rr = 0;
    memcpy(&rr, buf + (*pos), LEN);  
    *pos = (*pos) + LEN;
    return rr;
}


struct lib_node_n{
    std::string lib_;
    uint64_t beg_;
    uint64_t end_;
    lib_node_n(const std::string &lib, uint64_t beg, uint64_t end){
        lib_ = lib;
        beg_ = beg;
        end_ = end;
    }
};
typedef boost::shared_ptr<lib_node_n> lib_node;
struct pc_record_n{
    uint64_t number_;
    std::vector<uint64_t> pcs_;
    lib_node ln_;
    std::vector<uint64_t> mpcs_;
    pc_record_n(){
        number_ = 0;
    }
};
typedef boost::shared_ptr<pc_record_n> pc_record;


class cpu_profile_parser{
private:
public:
    bool parse(const std::string &file){
        // read file
        FILE *fl = fopen(file.c_str(), "rb");
        if(fl == NULL){
            fprintf(stderr, "error to open:%s\n", file.c_str());
            return false;
        }
        fseek(fl, 0, SEEK_END);
        int len = ftell(fl); 
        fseek(fl, 0, SEEK_SET);
        unsigned char *buf = new unsigned char[len];
        fread(buf, 1, len, fl);
        fclose(fl);
        
        int pos = 0;
        // binary header
        uint64_t hdr_count = read8(buf, &pos);
        if(hdr_count != 0){
            fprintf(stderr, "hdr_count(%d) != 0\n", hdr_count);
            return false;
        }
        uint64_t hdr_words = read8(buf, &pos);
        if(hdr_words < 3){
            fprintf(stderr, "hdr_words(%d) < 3\n", hdr_words);
            return false;
        } 
        uint64_t format_version = read8(buf, &pos);
        if(format_version != 0){
            fprintf(stderr, "format_version(%d) != 0\n", format_version);
            return false;
        }
        uint64_t sample_period = read8(buf, &pos);
        fprintf(stderr, "sample_period:%lu\n", sample_period);
        int64_t padding = read8(buf, &pos);
        if(padding != 0){
            fprintf(stderr, "padding(%d) != 0\n", padding);
            return false;
        }   
        uint64_t num = 0;
        std::vector<pc_record> prs;
        // record
        while(1){
            uint64_t sample_count = read8(buf, &pos);
            if(sample_count == 0){
                fprintf(stderr, "sample_count==0, maybe hit the binary trailer\n");
                uint64_t num_pcs = read8(buf, &pos);
                if(num_pcs != 1){
                    fprintf(stderr, "maybe hit binary trailer: num_pcs(%d) != 1\n", num_pcs);
                    return false;
                }
                uint64_t last = read8(buf, &pos);
                if(last != 0){
                    fprintf(stderr, "maybe hit binary trailer: last(%d) != 1\n", last);
                    return false;
                }
                fprintf(stderr, "read the end of binary\n");
                break;
            }           
            uint64_t num_pcs = read8(buf, &pos);
            if(num_pcs < 1){
                fprintf(stderr, "maybe hit binary trailer: num_pcs(%d) < 1\n", num_pcs);
                return false;
            }
            pc_record pr(new pc_record_n());
            pr->number_ = num++;
            for(uint64_t n = 0; n < num_pcs; n++){
                uint64_t pc = read8(buf, &pos);
                if(n > 0){
                    pc--;
                }
                pr->pcs_.push_back(pc);
            }
            prs.push_back(pr);
        } 
        // map
        std::string tail((char*)(buf + pos), len - pos);
        int opp = -1;
        std::vector<lib_node> lns;
        while(1){
            int pp = tail.find("\n", opp+1);
            if(pp  < 0){
                break;
            }
            std::string line = tail.substr(opp+1, pp-opp-1);
            opp = pp;
            fprintf(stderr, "%s\n", line.c_str());
            int blank = line.find(" ");
            if(blank < 0){
                fprintf(stderr, "can't find blank\n");
                break;
            }
            int slash = line.rfind("-", blank);
            if(slash < 0){
                fprintf(stderr, "can't find slash\n");
                break;
            }
            std::string beg = line.substr(0, slash);
            std::string end = line.substr(slash+1, blank-slash-1);
            uint64_t begn = 0, endn = 0;
            sscanf(beg.c_str(), "%016LX", &begn);
            sscanf(end.c_str(), "%016LX", &endn);
            blank = line.rfind(" ");
            std::string lib = line.substr(blank+1);
            fprintf(stderr, "\tlib:%s, beg:%s, end:%s, begn:%LX, endn:%LX\n", lib.c_str(), beg.c_str(), end.c_str(), begn, endn);
            lib_node ln(new lib_node_n(lib, begn, endn));
            lns.push_back(ln);
        } 
        // loop prs
        fprintf(stderr, "===================================================================\n");
        for(pc_record pr:prs){
            fprintf(stderr, "----------------------------------------------------------------\n");
            for(int i = 0; i < pr->pcs_.size(); i++){
                uint64_t pc = pr->pcs_[i];
                int n = 0;
                // find in witch lib
                for(n = 0; n < lns.size(); n++){
                    lib_node ln = lns[n];
                    if(ln->beg_<=pc && pc<=ln->end_){
                        // hit
                        pr->ln_ = ln;
                        uint64_t mpc = pc-ln->beg_;
                        if(ln->lib_ == "/usr/local/xyz/nredis/src/exphash"){// in primary app, mpc is pc
                            mpc = pc;
                        }
                        pr->mpcs_.push_back(mpc);
                        std::string symbol = get_symbol(ln->lib_, mpc);
                        fprintf(stderr, "pc:%LX, mpc:%LX, lib:%s, symbol:%s\n", pc, mpc, ln->lib_.c_str(), symbol.c_str());
                
                        break;
                    }
                } 
                if(n >= lns.size()){
                    fprintf(stderr, "has find ln, pc:%LX\n", pc);
                    pr->mpcs_.push_back(0);
                }
            }

            //break;
        }  
    } 

};
static void split_str(const std::string& input_str, const std::string &key, std::vector<std::string>& out_str_vec) {  
    if(input_str == ""){
        return;
    }
    int pos = input_str.find(key);
    int oldpos = -1;
    if(pos > 0){
        std::string tmp = input_str.substr(0, pos);
        out_str_vec.push_back(tmp);
    }
    while(1){
        if(pos < 0){
            break;
        }
        oldpos = pos;
        int newpos = input_str.find(key, pos + key.length());
        std::string tmp = input_str.substr(pos + key.length(), newpos - pos - key.length());
        out_str_vec.push_back(tmp);
        pos = newpos;
    }
    int tmplen = 0;
    if(out_str_vec.size() > 0){
        tmplen = out_str_vec.at(out_str_vec.size() - 1).length();
    }
    if(oldpos+tmplen < (int)input_str.length()-1){
        std::string tmp = input_str.substr(oldpos + key.length());
        out_str_vec.push_back(tmp);
    }
}
std::string get_symbol(const std::string &lib, uint64_t mpc){
    // run nm
    std::string rr = "";
    std::string cmd = "/usr/bin/nm  -n --demangle ";
    cmd += lib;
    cmd += " 2>>error.log > tmp.log";
    FILE *fl = NULL;
    int len = 0;
    for(int i = 0; i < 2; i++){
        system(cmd.c_str());
        // read
        fl = fopen("tmp.log", "rb");
        if(fl == NULL){
            return rr;
        }
        fseek(fl, 0, SEEK_END);
        len = ftell(fl);
        if(i==0 && len==0){
            cmd = "/usr/bin/nm -D -n --demangle ";
            cmd += lib;
            cmd += " 2>>error.log > tmp.log";
            fclose(fl);
            continue;
        }
        break;
    }
    fseek(fl, 0, SEEK_SET);
    char *buf = new char[len];
    fread(buf, 1, len, fl);
    fclose(fl);
    std::string bufs(buf, len);
    delete []buf;
    //
    std::vector<std::string> lines;
    split_str(bufs, "\n", lines);
    uint64_t last_addr = 0;
    std::string last_line = "";
    for(const std::string &line:lines){
        if(line.length() < 16){
            continue;
        } 
        std::string addrs = line.substr(0, 16);
        if(addrs == "               "){
            continue;
        }
        uint64_t addr = 0;
        sscanf(addrs.c_str(), "%LX", &addr); 
        if(mpc < addr){
            int pp = last_line.find(" ");
            pp = last_line.find(" ", pp+1);
            return last_line.substr(pp+1);
        }
        last_addr = addr;
        last_line = line;
    } 
    return rr;
}

int main(int argc, char **argv){
    cpu_profile_parser cpp;
    cpp.parse(argv[1]);
    return 0;
}
















