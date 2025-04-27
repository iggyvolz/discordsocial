<?php

namespace iggyvolz\discordsocial;

enum Status : int {
    case Disconnected = 0;
    case Connecting = 1;
    case Connected = 2;
    case Ready = 3;
    case Reconnecting = 4;
    case Disconnecting = 5;
    case HttpWait = 6;
}