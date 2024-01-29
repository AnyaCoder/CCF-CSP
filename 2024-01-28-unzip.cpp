// http://118.190.20.162/view.page?gpid=T168

#include <bits/stdc++.h>

using namespace std;

std::string hexToBinary(const std::string& hex) {
    // 将十六进制字符串转换为整数
    unsigned long long number = std::stoull(hex, nullptr, 16);

    // 计算十六进制数中的位数
    int numBits = hex.size() * 4;

    // 使用 std::bitset 转换为二进制字符串
    return std::bitset<64>(number).to_string().substr(64 - numBits);
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int n;
    cin >> n;
    string s, S;
    for (int i = 0; i < ceil(1.0 * n / 8.0); i++) {
        cin >> s;
        S += s;
    }
    
    // 1. 计算引导区域，解压缩后数据长度
    string boot_sector = "";
    for (int i = 0; i < n; i += 2) {
        string tmp = hexToBinary(S.substr(i, 2));
        if (tmp[0] == '1') {
            tmp[0] = '0';
            boot_sector += tmp;
        }
        else {
            boot_sector += tmp;
            break;
        }
    }

    int original_data_len = 0;
    int base256 = 1;
    for (int i = 0; i < boot_sector.length(); i += 8) {
        original_data_len += std::stoi(boot_sector.substr(i, 8), nullptr, 2) * base256;
        base256 *= 256;
    }
   // cout << "original_data_len: " << original_data_len << "\n";
    string buffer = "";
    for (int i = boot_sector.length() / 4; i < n * 2; ) {
        string tmp = hexToBinary(S.substr(i, 2));
      //  cout << "tmp: " << tmp << "\n";
        if (tmp[6] == '0' && tmp[7] == '0') {
            int l = std::stoull(tmp.substr(0, 6), nullptr, 2);
            l += 1;
            if (l <= 60) {
              //  cout << l << " <= 60: " ;
                int L = i + 2, R = i + 2 + l * 2;
                buffer += S.substr(L, R - L);
                // cout << S.substr(L, R - L) << "\n";
                i = R;
            }
            else {
                // 大的l要重新算
                int bl = 0;
                int base256 = 1;
                
                for (int j = i + 2; j < i + 2 + (l - 60) * 2; j += 2) {
                    bl += std::stoull(S.substr(j, 2), nullptr, 16) * base256;
                    base256 *= 256;
                }
                bl += 1;
              //  cout << l << " > 60: ";
              //  cout << "bl: " << bl << "\n";
                int L = i + 2 + (l - 60) * 2, R = i + 2 + (l - 60) * 2 + bl * 2;
                buffer += S.substr(L, R - L);
                //      cout << S.substr(L, R - L) << "\n";
                i = R;
            }
        }
        else if (tmp[6] == '0' && tmp[7] == '1') {
            int o, l;
        
            string tmph = hexToBinary(S.substr(i, 2));
            string tmpl = hexToBinary(S.substr(i + 2, 2));
            // cout << (tmph + tmpl) << "\n";
            string tmpo = tmph.substr(0, 3) + tmpl;
            break;
            o = std::stoull(tmpo, nullptr, 2);
            // cout << o << "\n";
            l = std::stoull(tmph.substr(3, 3), nullptr, 2);
            l += 4;
            string backs = buffer.substr(buffer.length() - o * 2, l * 2);
            string tmp_back = "";
            for (int j = 0; j < l * 2 / backs.length(); j++) {
                tmp_back += backs;
            }
            tmp_back += backs.substr(0, (2 * l) % backs.length());
           // cout << tmp_back << "\n";
            buffer += tmp_back;
            i += 4;
        }
        else if (tmp[6] == '1' && tmp[7] == '0') {
            int o, l;
            string tmp1 = hexToBinary(S.substr(i, 2));
            l = std::stoull(tmp1.substr(0, 6), nullptr, 2);
            l += 1;
            o = std::stoull(S.substr(i + 2, 2), nullptr, 16);
            o += std::stoull(S.substr(i + 4, 2), nullptr, 16) * 256;
            string backs = buffer.substr(buffer.length() - o * 2, l * 2);
            string tmp_back = "";
            for (int i = 0; i < l * 2 / backs.length(); i++) {
                tmp_back += backs;
            }
            tmp_back += backs.substr(0, (2 * l) % backs.length());
           // cout << tmp_back << "\n";
            buffer += tmp_back;
            i += 6;
        }
        else {
            cout << "last: " << S.substr(i) << "\n";
            break;
        }
    }

    for (int i = 0; i < ceil(1.0 * buffer.length() / 16); i += 1) {
        cout << buffer.substr(i * 16, 16) << "\n";
    }
    return 0;
}
