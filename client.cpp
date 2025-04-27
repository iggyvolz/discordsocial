#include <discordpp.h>
#include <iostream>
#include "util.hpp"
static void constructor(zend_execute_data *execute_data) {
    auto* client = new_object<discordpp::Client>(execute_data);
    new (client) discordpp::Client();
    original_zend_execute_ex(execute_data);
    client->SetApplicationId(504853468230451221);
}
static void destructor(zend_execute_data *execute_data) {
    get_object<discordpp::Client>(execute_data)->~Client();
    delete_object(execute_data);
}
static void run_callbacks(zend_execute_data *execute_data) {
    discordpp::RunCallbacks();
}
static std::vector<discordpp::Client::LogCallback> callbacks;
static void add_log_callback(zend_execute_data *execute_data) {
    auto* client = get_object<discordpp::Client>(execute_data);
    auto [closure, min_severity] = get_args<zend_object*, discordpp::LoggingSeverity>();
    zend_object * client_obj = execute_data->This.value.obj;
    client->AddLogCallback([closure, client_obj](const std::string& message, discordpp::LoggingSeverity severity) {
        call_closure_void(closure, std::array{
            to_zval(message),
            enum_c_to_php(severity, "iggyvolz\\discordsocial\\LoggingSeverity")
        }, client_obj);
    }, min_severity);
}
static void set_status_changed_callback(zend_execute_data *execute_data) {
    auto* client = get_object<discordpp::Client>(execute_data);
    auto* closure = get_arg<zend_object*>();
    zend_object * client_obj = execute_data->This.value.obj;
    client->SetStatusChangedCallback([closure, client_obj](discordpp::Client::Status status, discordpp::Client::Error error, int32_t errorDetail) {
        call_closure_void(closure, std::array{
            enum_c_to_php(status, "iggyvolz\\discordsocial\\Status"),
            enum_c_to_php(error, "iggyvolz\\discordsocial\\Error"),
            to_zval(errorDetail),
        }, client_obj);
    });
}
static void create_authorization_code_verifier(zend_execute_data* execute_data) {
    auto* client = get_object<discordpp::Client>(execute_data);
    to_zval(new_object<discordpp::AuthorizationCodeVerifier>("iggyvolz\\discordsocial\\AuthorizationCodeVerifier", client->CreateAuthorizationCodeVerifier()), *execute_data->return_value);
}
static void get_default_presence_scopes(const zend_execute_data* execute_data) {
    const std::string scopes = discordpp::Client::GetDefaultPresenceScopes();
    execute_data->return_value->value.arr = _zend_new_array_0();
    execute_data->return_value->u1.type_info = IS_ARRAY_EX;

    const char* start = scopes.c_str();
    const char* end = start + scopes.size();
    const char* pos = start;

    while (pos < end) {
        auto next = static_cast<const char*>(memchr(pos, ' ', end - pos));
        if (!next) next = end;

        add_next_index_stringl(execute_data->return_value, pos, next - pos);
        pos = next + 1;

        if (next == end) break;
    }

}
static void get_default_communication_scopes(const zend_execute_data* execute_data) {
    const std::string scopes = discordpp::Client::GetDefaultCommunicationScopes();
    execute_data->return_value->value.arr = _zend_new_array_0();
    execute_data->return_value->u1.type_info = IS_ARRAY_EX;

    const char* start = scopes.c_str();
    const char* end = start + scopes.size();
    const char* pos = start;

    while (pos < end) {
        auto next = static_cast<const char*>(memchr(pos, ' ', end - pos));
        if (!next) next = end;

        add_next_index_stringl(execute_data->return_value, pos, next - pos);
        pos = next + 1;

        if (next == end) break;
    }
}
static void authorize(zend_execute_data* execute_data) {
    auto* client = get_object<discordpp::Client>(execute_data);
    auto [client_id, scopes, challenge, callback, nonce, state] = get_args<uint64_t, std::vector<std::string>, discordpp::AuthorizationCodeChallenge*, zend_object*, std::optional<std::string>, std::optional<std::string>>();
    const std::string scopes_str = std::accumulate(scopes.begin() + 1, scopes.end(), scopes[0],
    [](const std::string& a, const std::string& b) {
        return a + ' ' + b;
    });

    discordpp::AuthorizationArgs args{};
    args.SetClientId(client_id);
    args.SetScopes(scopes_str);
    args.SetCodeChallenge(*challenge);
    args.SetNonce(nonce);
    args.SetState(state);
    zend_object * client_obj = execute_data->This.value.obj;
    client->Authorize(args, [callback, client_obj](const discordpp::ClientResult& result, const std::string& code, const std::string& redirectUri) {
        call_closure_void(callback, std::array{
            to_zval(new_object<discordpp::ClientResult>("iggyvolz\\discordsocial\\ClientResult", result)),
            to_zval(code),
            to_zval(redirectUri),
        }, client_obj);
    });
}
static void abort_authorize(zend_execute_data* execute_data) {
    get_object<discordpp::Client>(execute_data)->AbortAuthorize();
}
static void get_token(zend_execute_data* execute_data) {
    auto* client = get_object<discordpp::Client>(execute_data);
    auto [client_id, code, verifier, redirect_uri, callback] =
        get_args<uint64_t, std::string, std::string, std::string, zend_object*>();
    zend_object * client_obj = execute_data->This.value.obj;

    client->GetToken(client_id, code, verifier, redirect_uri, [callback, client_obj](
        const discordpp::ClientResult& result,
        const std::string& accessToken,
        const std::string& refreshToken,
        const discordpp::AuthorizationTokenType tokenType,
        const int32_t expiresIn,
        const std::string& scopes
        ) {
        call_closure_void(callback, std::array{
            to_zval(new_object<discordpp::ClientResult>("iggyvolz\\discordsocial\\ClientResult", result)),
            to_zval(accessToken),
            to_zval(refreshToken),
            enum_c_to_php(tokenType, "iggyvolz\\discordsocial\\AuthorizationTokenType"),
            to_zval(expiresIn),
            to_zval(scopes),
        }, client_obj);
    });
}
static void update_token(zend_execute_data* execute_data) {
    auto* client = get_object<discordpp::Client>(execute_data);
    auto [token_type, access_token, callback] =
        get_args<discordpp::AuthorizationTokenType, std::string, zend_object*>();
    zend_object * client_obj = execute_data->This.value.obj;

    client->UpdateToken(token_type, access_token, [callback, client_obj](const discordpp::ClientResult& result) {
        call_closure_void(callback, std::array{
            to_zval(new_object<discordpp::ClientResult>("iggyvolz\\discordsocial\\ClientResult", result)),
        }, client_obj);
    });
}
static void connect_(zend_execute_data* execute_data) {
    auto* client = get_object<discordpp::Client>(execute_data);
    client->Connect();
}
static void update_rich_presence(zend_execute_data* execute_data) {
    auto* client = get_object<discordpp::Client>(execute_data);
    auto [activity, callback] =
        get_args<discordpp::Activity*, zend_object*>();
    zend_object * client_obj = execute_data->This.value.obj;

    client->UpdateRichPresence(*activity, [callback, client_obj](const discordpp::ClientResult& result) {
        call_closure_void(callback, std::array{
            to_zval(new_object<discordpp::ClientResult>("iggyvolz\\discordsocial\\ClientResult", result)),
        }, client_obj);
    });
}
static void get_application_id(zend_execute_data* execute_data) {
    auto* client = get_object<discordpp::Client>(execute_data);
    to_zval(client->GetApplicationId(), *execute_data->return_value);
}
static void set_application_id(zend_execute_data* execute_data) {
    auto* client = get_object<discordpp::Client>(execute_data);
    client->SetApplicationId(get_arg<uint64_t>());
}
std::map<std::string, const std::function<void(zend_execute_data*)>> ClientHandlers = {
    {"__construct", constructor},
    {"__destruct", destructor},
    {"runCallbacks", run_callbacks},
    {"addLogCallback", add_log_callback},
    {"$statusChangedCallback::set", set_status_changed_callback},
    {"createAuthorizationCodeVerifier", create_authorization_code_verifier},
    {"getDefaultPresenceScopes", get_default_presence_scopes},
    {"getDefaultCommunicationScopes", get_default_communication_scopes},
    {"_authorize", authorize},
    {"abortAuthorize", abort_authorize},
    {"_getToken", get_token},
    {"_updateToken", update_token},
    {"connect", connect_},
    {"_updateRichPresence", update_rich_presence},
    {"get_application_id", get_application_id},
    {"set_application_id", set_application_id},
};
