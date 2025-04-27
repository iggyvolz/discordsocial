<?php

namespace iggyvolz\discordsocial;
enum LoggingSeverity: int {
    case Verbose = 1;
    case Info = 2;
    case Warning = 3;
    case Error = 4;
    case None = 5;
}