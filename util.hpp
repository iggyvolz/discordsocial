#pragma once
#include <functional>
#include <map>
#include <numeric>
#include <optional>
#include <ranges>
#include <stdexcept>

#include "php.h"
#include<string>
#include <utility>
#include <variant>
#include <zend_closures.h>

void* get_object(zend_long object_id);
void* new_object(zend_long object_id, std::size_t size);
void set_object(zend_long object_id, void* ptr);
void delete_object(zend_long object_id);

template<typename T>
T* get_object(zend_long object_id) {
    return static_cast<T*>(get_object(object_id));
}
template<typename T>
T* get_object(zend_execute_data *execute_data) {
    return get_object<T>(execute_data->This.value.obj->handle);
}
template<typename T>
[[maybe_unused]] T* new_object(zend_long object_id) {
    return static_cast<T*>(new_object(object_id, sizeof(T)));
}
template<typename T>
[[maybe_unused]] T* new_object(zend_execute_data *execute_data) {
    return new_object<T>(execute_data->This.value.obj->handle);
}
template<typename T, typename U>
[[maybe_unused]] T* new_object(zend_long object_id, U initial_value) {
    T* ptr = new_object<T>(object_id);
    *ptr = initial_value;
    return ptr;
}


static zend_string* zend_string_init(const std::string &string) {
    return zend_string_init(string.c_str(), string.size(), false);
}
template<typename F>
auto with_zend_string(const std::string& string, F&& callback) {
    using return_type = std::invoke_result_t<F, zend_string*>;
    zend_string *zend_str = zend_string_init(string);
    return_type val = callback(zend_str);
    zend_string_release(zend_str);
    return val;
}
template<typename T>
[[maybe_unused]] zend_object* new_object(const std::string& type, T initial_value) {
    zend_object* obj = zend_objects_new(with_zend_string(type, zend_lookup_class));
    new_object<T>(obj->handle, initial_value);
    return obj;
}
template<typename T>
[[maybe_unused]] T* new_object(zend_execute_data *execute_data, T initial_value) {
    T* ptr = new_object<T>(execute_data);
    *ptr = initial_value;
    return ptr;
}
static void delete_object(const zend_execute_data *execute_data) {
    delete_object(execute_data->This.value.obj->handle);
}

void register_class_handler(const std::string &class_name, const std::map<std::string, const std::function<void(zend_execute_data*)>>& method_handlers);
template<std::ranges::input_range Container>
    requires std::same_as<typename Container::value_type, std::string>
void register_class_handler(const Container &class_name, const std::map<std::string, const std::function<void(zend_execute_data*)>>& method_handlers) {
    register_class_handler(std::accumulate(
         next(begin(class_name)), // there is at least 1 element, so OK.
         end(class_name),
         class_name[0], // the initial value
         [](auto result, const auto &value) {
             return result + "\\" + value;
         }), method_handlers);
}
void register_function_handler(const std::string &function_name, std::function<void(zend_execute_data*)> function_handler);
template<std::ranges::input_range Container>
    requires std::same_as<typename Container::value_type, std::string>
void register_function_handler(const Container &class_name, std::function<void(zend_execute_data*)> function_handler) {
    register_function_handler(std::accumulate(
         next(begin(class_name)), // there is at least 1 element, so OK.
         end(class_name),
         class_name[0], // the initial value
         [](auto result, const auto &value) {
             return result + "\\" + value;
         }), function_handler);
}



template<typename T> requires std::is_enum_v<T>
T enum_php_to_c(zval zval) {
    return static_cast<T>(zend_read_property(zval.value.obj->ce, zval.value.obj, ZEND_STRL("value"), false, nullptr)->value.lval);
}
template<typename T> requires std::is_enum_v<T>
void enum_c_to_php(T enum_value, const std::string& enum_class, zval* result) {
    zend_class_entry *ce = with_zend_string(enum_class, zend_lookup_class);
    zend_function *func = with_zend_string("from", [ce](zend_string* str) {return zend_std_get_static_method(ce, str, nullptr);});
    zval param = {.value = {.lval = static_cast<zend_long>(enum_value)}, .u1 = {.type_info = IS_LONG}};
    zend_call_known_function(func, nullptr, ce, result, 1, &param, nullptr);
}
template<typename T> requires std::is_enum_v<T>
zval enum_c_to_php(T enum_value, const std::string& enum_class) {
    zval result;
    enum_c_to_php(enum_value, enum_class, &result);
    return result;
}

template<std::size_t N>
zval call_closure(zend_object* closure, std::array<zval, N> args, zend_object* scope) {
    zval* fci_args = args.data();
    zval retval = {.u1 = {.type_info = IS_UNDEF}};
    zend_fcall_info fci = {
        .size = sizeof(zend_fcall_info),
        .function_name = {.u1 = {.type_info = IS_UNDEF}},
        .retval = &retval,
        .params = fci_args,
        .object = closure,
        .param_count = N,
    };
    zend_fcall_info_cache fcc = {
        .function_handler = const_cast <zend_function*>(zend_get_closure_method_def(closure)),
        .calling_scope = scope->ce,
        .called_scope = scope->ce,
        .object = scope,
    };
    zend_call_function(&fci, &fcc);
    return retval;
}

template<std::size_t N>
zval call_closure(zend_object* closure, std::array<zval, N> args) {
    return call_closure(closure, args, nullptr);
}
template<std::size_t N>
void call_closure_void(zend_object* closure, std::array<zval, N> args, zend_object* scope) {
    zval retval = call_closure<N>(closure, args, scope);
    zval_ptr_dtor(&retval);
}
template<std::size_t N>
void call_closure_void(zend_object* closure, std::array<zval, N> args) {
    call_closure_void(closure, args, nullptr);
}

template<typename T>
T from_zval(const zval& in) {
    throw std::runtime_error(std::string("No handler for type in ") + __PRETTY_FUNCTION__ + " at " + std::to_string(__LINE__));
}
template<typename T>
void to_zval(T in, zval& out) {
    throw std::runtime_error(std::string("No handler for type in ") + __PRETTY_FUNCTION__ + " at " + std::to_string(__LINE__));
}
template<typename T>
zval to_zval(T in) {
    zval result;
    to_zval<T>(in, result);
    return result;
}
template<>
inline void to_zval<const std::string&>(const std::string& in, zval& out) {
    out.value.str = zend_string_init(in);
    out.u1.type_info = IS_STRING_EX;
}
template<>
inline void to_zval<const std::string>(const std::string in, zval& out) {
    out.value.str = zend_string_init(in);
    out.u1.type_info = IS_STRING_EX;
}
template<>
inline void to_zval<std::string>(std::string in, zval& out) {
    out.value.str = zend_string_init(in);
    out.u1.type_info = IS_STRING_EX;
}
template<>
inline void to_zval<bool>(bool in, zval& out) {
    out.u1.type_info = in ? IS_TRUE : IS_FALSE;
}
template<typename T> requires std::is_integral_v<T> && (!std::is_same_v<T, bool>)
void to_zval(T in, zval& out) {
    out.value.lval = in;
    out.u1.type_info = IS_LONG;
}
template<typename T> requires std::is_floating_point_v<T>
void to_zval(T in, zval& out) {
    out.value.dval = in;
    out.u1.type_info = IS_DOUBLE;
}
template<typename T> requires std::same_as<T, std::optional<typename T::value_type>>
void to_zval(T in, zval& out) {
    if (in.has_value()) {
        to_zval(in.value(), out);
    } else {
        out.u1.type_info = IS_NULL;
    }
}

template<>
inline void to_zval<zend_object*>(zend_object* in, zval& out) {
    out.value.obj = in;
    out.u1.type_info = IS_OBJECT_EX;
}

template<>
inline zval from_zval<zval>(const zval& in) {
    return in;
}
template<typename T> requires std::is_integral_v<T>
T from_zval(const zval& in) {
    return in.value.lval;
}
template<typename T> requires std::is_floating_point_v<T>
T from_zval(const zval& in) {
    return in.value.dval;
}
template<>
inline std::string from_zval<std::string>(const zval& in) {
    return {in.value.str->val, in.value.str->len};
}
template<typename T> requires std::is_enum_v<T>
T from_zval(const zval& in) {
    return enum_php_to_c<T>(in);
}
template<typename T> requires std::is_pointer_v<T>
T from_zval(const zval& in) {
    return get_object<std::remove_pointer_t<T>>(in.value.obj->handle);
}
template<>
inline zend_object* from_zval<zend_object*>(const zval& in) {
    return in.value.obj;
}
template<typename T> requires std::same_as<T, std::optional<typename T::value_type>>
auto from_zval(const zval& in) {
    return in.u1.type_info == IS_NULL ? std::nullopt : std::optional<typename T::value_type>(from_zval<typename T::value_type>(in));
}
static void foreach_hashtable(const HashTable& ht, const std::function<void(std::variant<std::string, zend_long>, const zval&)> &callback) {
    zend_long num_key;
    zend_string* str_key;
    zval* value;
    ZEND_HASH_FOREACH_KEY_VAL(&ht, num_key, str_key, value) {
        zval val;
        ZVAL_COPY_DEREF(&val, value);
        auto key =
            str_key ? std::variant<std::string, zend_long>(std::string{str_key->val, str_key->len}) : std::variant<std::string, zend_long>(num_key);
        callback(key, val);
    } ZEND_HASH_FOREACH_END();
}
static void foreach_hashtable(const HashTable& ht, const std::function<void(const zval&)> &callback) {
    zval* value;
    ZEND_HASH_FOREACH_VAL(&ht, value) {
        zval val;
        ZVAL_COPY_DEREF(&val, value);
        callback(val);
    } ZEND_HASH_FOREACH_END();
}
template<>
inline std::vector<zval> from_zval<std::vector<zval>>(const zval& in) {
    // Iterate through the array
    std::vector<zval> result;
    foreach_hashtable(*in.value.arr, [&result](const zval& value) {
        result.push_back(value);
    });
    return result;
}
template<typename T> requires std::same_as<T, std::vector<typename T::value_type>> && (!std::same_as<zval, typename T::value_type>)
T from_zval(const zval& in) {
    // P1206R7 - needs GCC 13
    // return map_arg<std::vector<zval>>(in)
    //     | std::ranges::views::transform([](const zval& zval) {return map_arg<typename T::value_type>(zval);})
    //     | std::ranges::to<std::vector>();
    T result;
    auto input = from_zval<std::vector<zval>>(in);
    std::transform(input.begin(), input.end(), std::back_inserter(result), [](const auto& zval) { return from_zval<typename T::value_type>(zval); });
    return result;
}

extern void (*original_zend_execute_ex) (zend_execute_data *execute_data);
template<std::size_t N>
std::array<zval, N> get_args() {
    std::array<zval, N> args;
    zend_get_parameters_array_ex(N, args.data());
    return args;
}

template<typename... Types>
std::tuple<Types...> get_args() {
    std::array<zval, sizeof...(Types)> raw_args = get_args<sizeof...(Types)>();
    return [&]<std::size_t... I>(std::index_sequence<I...>) {
        return std::make_tuple(from_zval<Types>(raw_args[I])...);
    }(std::index_sequence_for<Types...>{});
}


template<typename T>
T get_arg() {
    return from_zval<T>(get_args<1>().at(0));
}