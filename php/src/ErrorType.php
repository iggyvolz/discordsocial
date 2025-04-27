<?php

namespace iggyvolz\discordsocial;

enum ErrorType: int {
    case None = 0;
    case NetworkError = 1;
    case HTTPError = 2;
    case ClientNotReady = 3;
    case Disabled = 4;
    case ClientDestroyed = 5;
    case ValidationError = 6;
    case Aborted = 7;
    case AuthorizationFailed = 8;
    case RPCError = 9;
}