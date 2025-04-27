#include <discordpp.h>
#include "util.hpp"

static void destructor(zend_execute_data *execute_data) {
    get_object<discordpp::ClientResult>(execute_data)->~ClientResult();
    delete_object(execute_data);
}
static void to_string(zend_execute_data *execute_data) {
    discordpp::ClientResult* client_result = get_object<discordpp::ClientResult>(execute_data);
    to_zval(client_result->ToString(), *execute_data->return_value);
}
static void get_type(zend_execute_data* execute_data) {
    const auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    enum_c_to_php(client_result->Type(), "iggyvolz\\discordsocial\\ErrorType", execute_data->return_value);
}
static void set_type(zend_execute_data* execute_data) {
    auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    client_result->SetType(get_arg<discordpp::ErrorType>());
}
static void get_error(zend_execute_data* execute_data) {
    const auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    to_zval(client_result->Error(), *execute_data->return_value);
}
static void set_error(zend_execute_data* execute_data) {
    auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    client_result->SetError(get_arg<std::string>());
}
static void get_error_code(zend_execute_data* execute_data) {
    const auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    to_zval(client_result->ErrorCode(), *execute_data->return_value);
}
static void set_error_code(zend_execute_data* execute_data) {
    auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    client_result->SetErrorCode(get_arg<zend_long>());
}
static void get_status(zend_execute_data* execute_data) {
    const auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    enum_c_to_php(client_result->Status(), "iggyvolz\\discordsocial\\HttpStatusCode", execute_data->return_value);
}
static void set_status(zend_execute_data* execute_data) {
    auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    client_result->SetStatus(get_arg<discordpp::HttpStatusCode>());
}
static void get_response_body(zend_execute_data* execute_data) {
    const auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    to_zval(client_result->ResponseBody(), *execute_data->return_value);
}
static void set_response_body(zend_execute_data* execute_data) {
    auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    client_result->SetResponseBody(get_arg<std::string>());
}
static void get_successful(zend_execute_data* execute_data) {
    const auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    to_zval(client_result->Successful(), *execute_data->return_value);
}
static void set_successful(zend_execute_data* execute_data) {
    auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    client_result->SetSuccessful(get_arg<bool>());
}
static void get_retryable(zend_execute_data* execute_data) {
    const auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    to_zval(client_result->Retryable(), *execute_data->return_value);
}
static void set_retryable(zend_execute_data* execute_data) {
    auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    client_result->SetRetryable(get_arg<bool>());
}
static void get_retry_after(zend_execute_data* execute_data) {
    const auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    to_zval(client_result->RetryAfter(), *execute_data->return_value);
}
static void set_retry_after(zend_execute_data* execute_data) {
    auto* client_result = get_object<discordpp::ClientResult>(execute_data);
    client_result->SetRetryable(get_arg<float>());
}
std::map<std::string, const std::function<void(zend_execute_data*)>> ClientResultHandlers = {
    {"__destruct", destructor},
    {"__toString", to_string},
    {"$type::get", get_type},
    {"$type::set", set_type},
    {"$error::get", get_error},
    {"$error::set", set_error},
    {"$errorCode::get", get_error_code},
    {"$errorCode::set", set_error_code},
    {"$status::get", get_status},
    {"$status::set", set_status},
    {"$responseBody::get", get_response_body},
    {"$responseBody::set", set_response_body},
    {"$successful::get", get_successful},
    {"$successful::set", set_successful},
    {"$retryable::get", get_retryable},
    {"$retryable::set", set_retryable},
    {"$retryAfter::get", get_retry_after},
    {"$retryAfter::set", set_retry_after},

};
