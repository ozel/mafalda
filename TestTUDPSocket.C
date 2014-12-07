#include "TUDPSocket.h"
#include "TString.h"

//
// As test echo server use udpserver.c in the same directory.
// To compile it do:
//    clang udpserver.c -o udpserver
//

// int createTServerSocket(){
//    printf("testTSocket: Creating TSocket\n");
//    TServerSocket * fServerSocket = new TServerSocket(1501, 0, 0, -1, "UDP");
//
//    TMonitor *mon = new TMonitor;
//
//    mon->Add(fServerSocket);
//
//    TSocket *s0 = 0;
//
//  //  while (1) {
//       char msgRaw[1024] = {0};
//       TSocket  *s;
//
//       s = mon->Select();
//
//       if (s->IsA() == TServerSocket::Class()) {
//          if (!s0) {
//             s0 = ((TServerSocket *)s)->Accept();
//             s0->Send("go 0");
//             mon->Add(s0);
//          }
//
//          if (s0) {
//             mon->Remove(ss);
//             ss->Close();
//          }
//          continue;
//       }
//
//       s->RecvRaw((void *) msgRaw, 1024);
//       printf("Server Message Received %s\n", msgRaw);
//       s->SendRaw(msgRaw, 1024);
//
// // }/* end of server infinite loop */
//
//    s0->Close();
//
//    return 1;
// }

int TestTUDPSocket()
{
   printf("testTSocket: Creating TUDPSocket\n");
   TUDPSocket * fSocket = new TUDPSocket("ozelipad", 8123);

   if (!fSocket || !fSocket->IsValid()) {
      Error("testTSocket","cannot connect to localhost");
      return -1;
   }

   TString msg = "80	80	80\n81	80	80\n82	80	80\n83	80	80\n84	80	80\n85	80	80\n";

   fSocket->SetOption(kNoBlock, 1);


   printf("new frame:\n%s",msg.Data());

   if (fSocket->SendRaw(msg.Data(), msg.Length()) == -1) {
      Error("testTSocket", "error sending command to host %s", fServer.GetHost());
      return -1;
   }

   char msgRaw[1024] = {0};
   return 1;
}
