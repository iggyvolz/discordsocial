#include <discordpp.h>
#include "util.hpp"
static void method_get(zend_execute_data *execute_data) {
    const auto* challenge = get_object<discordpp::AuthorizationCodeChallenge>(execute_data);
    enum_c_to_php(challenge->Method(), "iggyvolz\\discordsocial\\AuthenticationCodeChallengeMethod", execute_data->return_value);
}
static void method_set(zend_execute_data *execute_data) {
    auto* challenge = get_object<discordpp::AuthorizationCodeChallenge>(execute_data);
    challenge->SetMethod(get_arg<discordpp::AuthenticationCodeChallengeMethod>());
}
static void challenge_get(zend_execute_data *execute_data) {
    const auto* challenge = get_object<discordpp::AuthorizationCodeChallenge>(execute_data);
    to_zval(challenge->Challenge(), *execute_data->return_value);
}
static void challenge_set(zend_execute_data *execute_data) {
    auto* challenge = get_object<discordpp::AuthorizationCodeChallenge>(execute_data);
    challenge->SetChallenge(get_arg<std::string>());
}
std::map<std::string, const std::function<void(zend_execute_data*)>> AuthorizationCodeChallengeHandlers = {
    {"$method::get", method_get},
    {"$method::set", method_set},
    {"$challenge::get", challenge_get},
    {"$challenge::set", challenge_set},
};
