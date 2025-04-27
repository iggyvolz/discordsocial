
#include "util.hpp"

#include <functional>

static std::map<zend_long, void*> objects;

void* get_object(zend_long object_id) {
    return objects[object_id];
}
void set_object(zend_long object_id, void* ptr) {
    objects[object_id] = ptr;
}
void* new_object(zend_long object_id, std::size_t size) {
    void* ptr = emalloc(size);
    objects[object_id] = ptr;
    return ptr;
}
void delete_object(zend_long object_id) {
    efree(objects[object_id]);
    objects.erase(object_id);
}

void (*original_zend_execute_ex) (zend_execute_data *execute_data) = nullptr;

static std::map<std::string,std::map<std::string, const std::function<void(zend_execute_data*)>>> class_method_handlers;
static std::map<std::string, std::function<void(zend_execute_data*)>> function_handlers;

void my_execute_ex (zend_execute_data *execute_data) {
    if (execute_data->func && execute_data->func->common.function_name) {
        if (execute_data->func->common.scope) {
            // Method invocation
            const std::string class_name = ZSTR_VAL(execute_data->func->common.scope->name);
            const std::string method_name = ZSTR_VAL(execute_data->func->common.function_name);
            if (class_method_handlers.contains(class_name) && class_method_handlers[class_name].contains(method_name)) {
                class_method_handlers[class_name][method_name](execute_data);
                return;
            }
        } else {
            // Function invocation
            const std::string function_name = ZSTR_VAL(execute_data->func->common.function_name);
            if (function_handlers.contains(function_name)) {
                function_handlers[function_name](execute_data);
                return;
            }
        }
    }
    original_zend_execute_ex(execute_data);
}


static void register_hook() {
    if (original_zend_execute_ex == nullptr) {
        original_zend_execute_ex = zend_execute_ex;
        zend_execute_ex = my_execute_ex;
    }
}

void register_class_handler(const std::string &class_name, const std::map<std::string, const std::function<void(zend_execute_data*)>>& method_handlers) {
    register_hook();
    class_method_handlers[class_name] = method_handlers;
}
void register_function_handler(const std::string &function_name, const std::function<void(zend_execute_data*)>& function_handler) {
    register_hook();
    function_handlers[function_name] = function_handler;
}
