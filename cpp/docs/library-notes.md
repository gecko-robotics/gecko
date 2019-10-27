## ZeroMQ [`cppzmq`]

I use a high level C++ abstraction of the lower level zmq to build gecko.

- macOS: `brew install zeromq`
- linux: `sudo apt install libzmq3-dev`

## Boost

I generally hate boost because it is a pain to work with. Right now you don't
need it.

- macOS: `brew install boost`
- linux: `sudo apt install libboost-dev`

## Core

This is the main message hub. GeckoCore also is passed the PIDs for processes on the
local machine and prints performance data on each process:

```bash
+========================================
| Processes Performance
| [24790] GeckoCore............. cpu:  0.3%  mem:  0.0%
| [24793] pub_ryan.............. cpu:  0.1%  mem:  0.0%
| [24795] pub_mike.............. cpu:  0.1%  mem:  0.0%
| [24796] sub_mike.............. cpu: 20.5%  mem:  0.0%
| [24797] pub_sammie............ cpu:  0.1%  mem:  0.0%
| [24798] sub_sammie............ cpu: 20.5%  mem:  0.0%
+------------------------------
| ESTABLISHED Connections
| pub_mike............ 192.168.86.22:50551 --> 192.168.86.22:50557
| sub_mike............ 192.168.86.22:50557 --> 192.168.86.22:50551
| pub_sammie.......... 192.168.86.22:50554 --> 192.168.86.22:50558
| sub_sammie.......... 192.168.86.22:50558 --> 192.168.86.22:50554
+------------------------------
| LISTEN Connections
| GeckoCore........... 192.168.86.22:11311
| pub_ryan............ 192.168.86.22:50548
| pub_mike............ 192.168.86.22:50551
| pub_sammie.......... 192.168.86.22:50554
+========================================
| Published Topics <topic>@tcp://<ip>:<port>
|  1: ryan@tcp://192.168.86.22:50548
|  2: mike@tcp://192.168.86.22:50551
|  3: sammie@tcp://192.168.86.22:50554
+========================================
```

```bash
========================================
 Geckocore [65975]
-------------
 Key: local
 Host IP: 10.0.1.57
 Listening on: 224.3.29.110:11311
-------------
Known Services [6]
 * hello:........................ tcp://10.0.1.57:65303
 * hey there:.................... tcp://10.0.1.57:65304
 * ryan:......................... tcp://10.0.1.57:65310
 * mike:......................... tcp://10.0.1.57:65311
 * sammie:....................... tcp://10.0.1.57:65312
 * scott:........................ tcp://10.0.1.57:65313

Binders [6]
 [65993] hello................. cpu:  0.0%  mem:  0.0%
 [65994] hey there............. cpu:  0.0%  mem:  0.0%
 [66008] ryan.................. cpu:  0.1%  mem:  0.0%
 [66010] mike.................. cpu:  0.1%  mem:  0.0%
 [66012] sammie................ cpu:  0.1%  mem:  0.0%
 [66014] scott................. cpu:  0.1%  mem:  0.0%

Connections [8]
 [65995] hello................. cpu: 20.7%  mem:  0.0%
 [65996] hello................. cpu: 20.9%  mem:  0.0%
 [65997] hey there............. cpu: 21.0%  mem:  0.0%
 [65998] hey there............. cpu: 20.8%  mem:  0.0%
 [66011] mike.................. cpu: 19.0%  mem:  0.0%
 [66013] sammie................ cpu: 19.0%  mem:  0.0%
 [66015] scott................. cpu: 19.4%  mem:  0.0%
 [66009] ryan.................. cpu: 18.7%  mem:  0.0%
```



## Performance

`geckocore` has a thread that prints out performance information of all
processes that have talked to it.

```
========================================
 Geckocore [96103]
-------------
 Start: 09-06-2019 09:03:49
 Key: local
 Host IP: 10.0.1.70
 Listening on: 224.3.29.110:11311
 CPU: 0.0   Memory: 0.0
--------------------
 Binder [4]
 Topic                           CPU  MEM  EndPt
 > mike[97311]...................0.0  0.0  tcp://10.0.1.70:51175
 > ryan[97305]...................0.0  0.0  tcp://10.0.1.70:51174
 > sammie[97315].................0.0  0.0  tcp://10.0.1.70:51176
 > scott[97320]..................0.0  0.0  tcp://10.0.1.70:51177
--------------------
 Connector [8]
 Topic                           CPU  MEM  EndPt
 > ryan[97307]...................20.5 0.0  tcp://10.0.1.70:51174
 > ryan[97308]...................22.2 0.0  tcp://10.0.1.70:51174
 > mike[97312]...................20.5 0.0  tcp://10.0.1.70:51175
 > mike[97313]...................16.5 0.0  tcp://10.0.1.70:51175
 > sammie[97316].................23.7 0.0  tcp://10.0.1.70:51176
 > sammie[97318].................22.0 0.0  tcp://10.0.1.70:51176
 > scott[97321]..................25.8 0.0  tcp://10.0.1.70:51177
 > scott[97323]..................25.9 0.0  tcp://10.0.1.70:51177
```

There are 4 publishers and 8 subscribers (2 per topic):

```
(cv) kevin@Logan tcp $ ./tcp.py
----------------------------------
GeckoPy
-----------
  Process: pub_ryan
  PID: 97305
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_ryan
  PID: 97307
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_ryan_2
  PID: 97308
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: pub_mike
  PID: 97311
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_mike
  PID: 97312
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_mike_2
  PID: 97313
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: pub_sammie
  PID: 97315
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_sammie
  PID: 97316
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_sammie_2
  PID: 97318
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: pub_scott
  PID: 97320
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_scott
  PID: 97321
  Host: 10.0.1.70
----------------------------------
----------------------------------
GeckoPy
-----------
  Process: sub_scott_2
  PID: 97323
  Host: 10.0.1.70
----------------------------------
>> bind raw: ['local', 'ryan', '97305', 'ok']
>> pub_ryan:[0] published msg
>> conn raw:  ['']
>> conn raw:  ['local', 'ryan', 'tcp://10.0.1.70:51174', 'ok']
[>] Receiving messages on ALL topics...
>> conn raw:  ['local', 'ryan', 'tcp://10.0.1.70:51174', 'ok']
[>] Receiving messages on ALL topics...
>> bind raw: ['local', 'mike', '97311', 'ok']
>> pub_mike:[0] published msg
>> conn raw:  ['local', 'mike', 'tcp://10.0.1.70:51175', 'ok']
[>] Receiving messages on ALL topics...
>> conn raw:  ['local', 'mike', 'tcp://10.0.1.70:51175', 'ok']
[>] Receiving messages on ALL topics...
>> bind raw: ['local', 'sammie', '97315', 'ok']
>> pub_sammie:[0] published msg
>> conn raw:  ['']
>> conn raw:  ['local', 'sammie', 'tcp://10.0.1.70:51176', 'ok']
[>] Receiving messages on ALL topics...
>> conn raw:  ['local', 'sammie', 'tcp://10.0.1.70:51176', 'ok']
[>] Receiving messages on ALL topics...
>> bind raw: ['local', 'scott', '97320', 'ok']
>> pub_scott:[0] published msg
>> conn raw:  ['local', 'scott', 'tcp://10.0.1.70:51177', 'ok']
[>] Receiving messages on ALL topics...
>> conn raw:  ['local', 'scott', 'tcp://10.0.1.70:51177', 'ok']
[>] Receiving messages on ALL topics...
>> pub_ryan:[1] published msg
>> sub_ryan:ryan: {'time': 0.4579482078552246}
>> sub_ryan_2:ryan: {'time': 0.4579482078552246}
>> pub_mike:[1] published msg
>> sub_mike:mike: {'time': 0.4434690475463867}

... more messages streaming by

>> sub_sammie:sammie: {'time': 6.8596580028533936}
>> sub_sammie:sammie: {'time': 6.8596580028533936}
>> sub_scott:scott: {'time': 6.861537933349609}
>> sub_scott_:scott: {'time': 6.861537933349609}
>> sub_ryan:ryan: {'time': 6.969848155975342}
>> pub_ryan:[15] published msg
>> pub_mike:[15] published msg
>> sub_ryan_2:ryan: {'time': 7.471033096313477}
>> sub_mike:mike: {'time': 7.450994253158569}
>> sub_mike_2:mike: {'time': 7.450994253158569}
>> pub_sammie:[15] published msg
>> pub_scott:[15] published msg
>> sub_sammie:sammie: {'time': 7.360673904418945}
>> sub_sammie:sammie: {'time': 7.360673904418945}
>> sub_scott:scott: {'time': 7.3624796867370605}
>> sub_scott_:scott: {'time': 7.3624796867370605}
^Cmain process got ctrl-c
sub bye ...
pub bye ...
sub bye ...
pub bye ...
sub bye ...
sub bye ...
pub bye ...
pub bye ...
sub bye ...
sub bye ...
sub bye ...
sub bye ...
```
