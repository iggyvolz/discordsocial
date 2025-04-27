#include <discordpp.h>
#include "util.hpp"
static void verifier_get(zend_execute_data *execute_data) {
    const auto* verifier = get_object<discordpp::AuthorizationCodeVerifier>(execute_data);
    to_zval(verifier->Verifier(), *execute_data->return_value);
}
static void verifier_set(zend_execute_data *execute_data) {
    auto* verifier = get_object<discordpp::AuthorizationCodeVerifier>(execute_data);
    verifier->SetVerifier(get_arg<std::string>());
}
static void challenge_get(zend_execute_data *execute_data) {
    const auto* verifier = get_object<discordpp::AuthorizationCodeVerifier>(execute_data);
    to_zval(new_object<discordpp::AuthorizationCodeChallenge>("iggyvolz\\discordsocial\\AuthorizationCodeChallenge", verifier->Challenge()), *execute_data->return_value);
}
static void challenge_set(zend_execute_data *execute_data) {
    auto* verifier = get_object<discordpp::AuthorizationCodeVerifier>(execute_data);
    verifier->SetChallenge(get_arg<discordpp::AuthorizationCodeChallenge>());
}
std::map<std::string, const std::function<void(zend_execute_data*)>> AuthorizationCodeVerifierHandlers = {
    {"$verifier::get", verifier_get},
    {"$verifier::set", verifier_set},
    {"$challenge::get", challenge_get},
    {"$challenge::set", challenge_set},
};
