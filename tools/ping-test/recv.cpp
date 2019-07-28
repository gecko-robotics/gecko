/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2019 Kevin Walchko
* see LICENSE for full details
\**************************************************/
#include <gecko/db.hpp>
#include <gecko/gecko.hpp>
// #include <gecko/msocket.hpp>
#include <gecko/time.hpp>

#include <iostream>
#include <string>
#include <stdint.h>
// #include <thread>

using namespace std;


int main(){
    // SSocket ss;
    // string maddr = "224.3.29.110";
    // uint16_t port = 11311;
    // ss.init(maddr, port, 1, true);
    //
    // bool ok = true;
    // // Ascii a;
    //
    // cout << "\n==================================" << endl;
    // cout << " " << maddr << ":" << port << endl;
    // cout << " Receiving:" << endl;
    //
    // while(ok){
    //     // string ans = ss.recv_nb();
    //     // MsgAddr ma = ss.recv2();
    //     // string ans = ma.msg;
    //     string ans;
    //     struct sockaddr_in addr;
    //
    //     tie(ans, addr) = ss.recv_nb();
    //
    //     if(!ans.empty()){
    //         // ascii_t t = a.unpack(ans);
    //
    //         cout << ">> Msg recv'ed: " << ans << " from " <<
    //             inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << endl;
    //         // for (const auto& s: t) cout << s << " ";
    //         // cout << endl;
    //         // string msg;
    //
    //         // if (t.size() == 3) msg = handle_conn(t);
    //         // else if (t.size() == 4) msg = handle_bind(t);
    //         // ss.send(msg, addr);
    //
    //         ok = false;
    //     }
    //     // else cout << "** nothing **" << endl;
    //
    //     gecko::msleep(100);
    // }
    return 0;
}
