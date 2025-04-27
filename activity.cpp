#include <discordpp.h>
#include <iostream>
#include "util.hpp"
static void constructor(zend_execute_data *execute_data) {
    auto* client = new_object<discordpp::Activity>(execute_data);
    new (client) discordpp::Activity();
    original_zend_execute_ex(execute_data);
}
static void destructor(zend_execute_data *execute_data) {
    get_object<discordpp::Activity>(execute_data)->~Activity();
    delete_object(execute_data);
}
static void name_get(zend_execute_data *execute_data) {
    const auto* activity = get_object<discordpp::Activity>(execute_data);
    to_zval(activity->Name(), *execute_data->return_value);
}
static void name_set(zend_execute_data *execute_data) {
    auto* activity = get_object<discordpp::Activity>(execute_data);
    activity->SetName(get_arg<std::string>());
}
static void application_id_get(zend_execute_data *execute_data) {
    const auto* activity = get_object<discordpp::Activity>(execute_data);
    to_zval(activity->ApplicationId(), *execute_data->return_value);
}
static void application_id_set(zend_execute_data *execute_data) {
    auto* activity = get_object<discordpp::Activity>(execute_data);
    activity->SetApplicationId(get_arg<std::optional<uint64_t>>());
}
std::map<std::string, const std::function<void(zend_execute_data*)>> ActivityHandlers = {
    {"__construct", constructor},
    {"__destruct", destructor},
    {"$name::get", name_get},
    {"$name::set", name_set},
    {"$applicationId::get", application_id_get},
    {"$applicationId::set", application_id_set},
};
