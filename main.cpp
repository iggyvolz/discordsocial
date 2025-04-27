#include <iostream>
#include <map>
#include<string>
#include "php.h"
#include "util.hpp"
#include "discordpp.h"

extern std::map<std::string, const std::function<void(zend_execute_data *)>> ClientHandlers;
extern std::map<std::string, const std::function<void(zend_execute_data *)>> AuthorizationCodeVerifierHandlers;
extern std::map<std::string, const std::function<void(zend_execute_data *)>> AuthorizationCodeChallengeHandlers;
extern std::map<std::string, const std::function<void(zend_execute_data *)>> ClientResultHandlers;
extern std::map<std::string, const std::function<void(zend_execute_data *)>> ActivityHandlers;
zend_result minit(int type, int module_number) {
    register_class_handler(std::vector<std::string>{"iggyvolz", "discordsocial", "Client"}, ClientHandlers);
    register_class_handler(std::vector<std::string>{"iggyvolz", "discordsocial", "AuthorizationCodeVerifier"}, AuthorizationCodeVerifierHandlers);
    register_class_handler(std::vector<std::string>{"iggyvolz", "discordsocial", "AuthorizationCodeChallenge"}, AuthorizationCodeChallengeHandlers);
    register_class_handler(std::vector<std::string>{"iggyvolz", "discordsocial", "ClientResult"}, ClientResultHandlers);
    register_class_handler(std::vector<std::string>{"iggyvolz", "discordsocial", "Activity"}, ActivityHandlers);
    return SUCCESS;
}

zend_result mshutdown(int type, int module_number) {
    return SUCCESS;
}
std::string discord_version;

zend_module_entry module_entry = {
    .size = sizeof(zend_module_entry),
    .zend_api = ZEND_MODULE_API_NO,
    .zend_debug = ZEND_DEBUG,
    .zts = USING_ZTS,
    .name = "discordsocial",
    .module_startup_func = minit,
    .version = discord_version.c_str(),
    .build_id = ZEND_MODULE_BUILD_ID
};
extern "C" {
    ZEND_DLEXPORT zend_module_entry *get_module(void) {
        discord_version = (std::to_string(Discord_Client_GetVersionMajor()) + "." + std::to_string(Discord_Client_GetVersionMinor()) + "." + std::to_string(Discord_Client_GetVersionPatch()));
        return &module_entry;
    }
}
