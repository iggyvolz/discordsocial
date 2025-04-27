<?php

namespace iggyvolz\discordsocial;

final class Activity {
    public function __construct() {}
    public function __destruct() {}
    public string $name {get{} set{}}
//    public ActivityTypes $types {get{} set{}}
//    public ?string $state {get{} set{}}
//    public ?string $details {get{} set{}}
    public ?int $applicationId {get{} set{}}
//    public ?ActivityAssets $assets {get{} set{}}
//    public ?ActivityTimestamps $timestamps {get{} set{}}
//    public ?ActivityParty $party {get{} set{}}
//    public ?ActivitySecrets $secrets {get{} set{}}
//    public ActivityGamePlatforms $supportedPlatforms {get{} set{}}
}