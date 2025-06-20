#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <thread>

namespace detail
{
    template <class... Fs>
    struct overload;

    template <class F>
    struct overload<F> : public F
    {
        explicit overload(F f) : F(f)
        {
        }
    };
    template <class F, class... Fs>
    struct overload<F, Fs...>
        : public overload<F>, public overload<Fs...>
    {
        overload(F f, Fs... fs) : overload<F>(f), overload<Fs...>(fs...)
        {
        }
        using overload<F>::operator();
        using overload<Fs...>::operator();
    };
}

template <class... F>
auto overloaded(F... f)
{
    return detail::overload<F...>(f...);
}

namespace td_api = td::td_api;


class TdBasic
{
public:
    TdBasic()
    {
        td::ClientManager::execute(td_api::make_object<td_api::setLogVerbosityLevel>(1));
        client_manager_ = std::make_unique<td::ClientManager>();
        client_id_ = client_manager_->create_client_id();
        send_query(td_api::make_object<td_api::getOption>("version"), {});
    }

    
    void loop()
    {
        while (true)
        {
            if (need_restart_)
            {
                restart();
            }
            else if (!are_authorized_)
            {
                process_response(client_manager_->receive(10));
            }
            else{
                std::cout << "Enter action [q] quit [c] show chats [m <chat_id> "
                             "<text>] send message [me] show self [l] logout:  [f]show message logs(f <chat_id>)"
                        << std::endl;
                std::string line;
                std::getline(std::cin, line);
                std::istringstream ss(line);
                std::string action;
                if (!(ss >> action))
                {
                    continue;
                }
                if (action == "q")
                {
                    return;
                }
                else if (action == "close")
                {
                    std::cout << "Closing..." << std::endl;
                    send_query(td::td_api::make_object<td_api::close>(), {});
                }
                else if (action == "me")
                {
                    send_query(td::td_api::make_object<td_api::getMe>(),
                               [this](Object object)
                               { std::cout << to_string(object) << std::endl; });
                }
                else if (action == "l")
                {
                    std::cout << "Logging out..." << std::endl;
                    send_query(td_api::make_object<td_api::logOut>(), {});
                }
                else if (action == "m")
                {
                    std::int64_t chat_id;
                    ss >> chat_id;
                    ss.get();
                    std::string text;
                    std::getline(ss, text);
                    std::cout << "Sending message to chat " << chat_id << "...." << std::endl;
                    auto send_message = td_api::make_object<td_api::sendMessage>();
                    send_message->chat_id_ = chat_id;
                    auto message_content = td_api::make_object<td_api::inputMessageText>();
                    message_content->text_ = td_api::make_object<td::td_api::formattedText>();
                    message_content->text_->text_ = std::move(text);
                    send_message->input_message_content_ = std::move(message_content);
                    send_query(std::move(send_message), {});
                }
                else if (action == "f"){
                    std::int64_t chat_id;
                    ss >> chat_id;
                    ss.get();
                    std::cout << "Openning chat " <<chat_id << " " <<std::endl;
                    
                    auto getchatlog = td_api::make_object<td_api::getChatHistory>();
                    getchatlog->chat_id_= chat_id;
                    getchatlog->from_message_id_ = 0;
                    getchatlog->offset_ = 0;
                    getchatlog->limit_ = 20;
                    getchatlog->only_local_ = false;
                    
                    send_query(std::move(getchatlog), [this](Object object)
                    {
                        if(object->get_id() == td_api::error::ID){
                            auto error = td::move_tl_object_as<td_api::error>(object);
                            std::cerr << "Failed to get chat history: " << error->message_ << std::endl;
                            return;
                        }
                        
                        auto messages_ptr = td::move_tl_object_as<td_api::messages>(object);
                        int64_t xuser;
                        std::string msg;
                        for (auto &message : messages_ptr->messages_) {
                        
                            if(message == nullptr){continue;}
                            td_api::int64 sender_id;
                            
                            auto sender_user = static_cast<td_api::messageSenderUser*>(message->sender_id_.get());
                            sender_id = sender_user->user_id_;
                            td_api::downcast_call(*message->content_, overloaded(
                                [&message,&xuser,&msg](td_api::messageText &text_content) {
                                    td_api::downcast_call(*message->sender_id_, overloaded(
                                        [&xuser](td_api::messageSenderUser &user) {
                                            //std::cout << "From User: " << user.user_id_ << std::endl;
                                            xuser = std::move(user.user_id_); 
                                        },
                                        [](td_api::messageSenderChat &chat) {
                                            std::cout <<"From Chat: " << chat.chat_id_ << std::endl;
                                        }
                                    ));

                                    msg = std::move(text_content.text_->text_);
                                },
                                [](auto &other) {
                                    std::cout << "Non-text message: " << td_api::to_string(other) << std::endl;
                                }
                            ));
                            std::cout<<chat_title_[xuser]<<": "<<msg<<std::endl;
                        }
                    });
                }
                else if (action == "c")
                {

                    std::cout << "Loading chat list..." << std::endl;
                    send_query(td_api::make_object<td_api::getChats>(nullptr, 10), [this](Object object)
                    {
                        if (object->get_id() == td_api::error::ID){
                            return;
                        }
                        auto chats = td::move_tl_object_as<td_api::chats>(object);
                        std::cout<<"Total count: "<<chats->total_count_<<std::endl;
                        for(auto chat_id : chats->chat_ids_) {
                            std::cout << "[chat_id" << chat_id << "] [title:" <<chat_title_[chat_id] << "]" <<std::endl;
                        } 
                    });
                }
            }
        }
    }

private:
    using Object = td_api::object_ptr<td_api::Object>;
    std::unique_ptr<td::ClientManager> client_manager_;
    std::int32_t client_id_{0};

    td_api::object_ptr<td_api::AuthorizationState> authorization_state_;
    bool are_authorized_{false};
    bool need_restart_{false};
    std::uint64_t current_query_id_{0};
    std::uint64_t authentication_query_id_{0};

    std::map<std::uint64_t, std::function<void(Object)>> handlers_;

    std::map<std::int64_t, td_api::object_ptr<td_api::user>> users_;

    std::map<std::int64_t, std::string> chat_title_;

    void restart()
    {
        client_manager_.reset();
        *this = TdBasic();
    }

    void send_query(td_api::object_ptr<td_api::Function> f, std::function<void(Object)> handler)
    {
        auto query_id = next_query_id();
        if (handler)
        {
            handlers_.emplace(query_id, std::move(handler));
        }
        client_manager_->send(client_id_, query_id, std::move(f));

        autoupdate();
    }

    void process_response(td::ClientManager::Response response)
    {
        if (!response.object)
        {
            return;
        }

        if (response.request_id == 0)
        {
            return process_update(std::move(response.object));
        }
        auto it = handlers_.find(response.request_id);
        if (it != handlers_.end())
        {
            it->second(std::move(response.object));
            handlers_.erase(it);
        }
    }

    void autoupdate(){
        std::cout << "Checking for updates..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        while (true)
        {
            auto response = client_manager_->receive(0);
            if (response.object)
            {
                process_response(std::move(response));
            }
            else
            {
                break;
            }
        }
    }

    std::string get_user_name(std::int64_t user_id) const
    {
        auto it = users_.find(user_id);
        if (it == users_.end())
        {
            return "unknown user";
        }
        return it->second->first_name_ + " " + it->second->last_name_;
    }

    std::string get_chat_title(std::int64_t chat_id) const
    {
        auto it = chat_title_.find(chat_id);
        if (it == chat_title_.end())
        {
            return "unknown chat";
        }
        return it->second;
    }
    void process_update(td_api::object_ptr<td_api::Object> update)
    {
        td_api::downcast_call(
            *update, overloaded(
                         [this](td_api::updateAuthorizationState &update_authorization_state)
                         {
                             authorization_state_ = std::move(update_authorization_state.authorization_state_);
                             on_authorization_state_update();
                         },
                         [this](td_api::updateNewChat &update_new_chat)
                         {
                             chat_title_[update_new_chat.chat_->id_] = update_new_chat.chat_->title_;
                         },
                         [this](td_api::updateChatTitle &update_chat_title)
                         {
                             chat_title_[update_chat_title.chat_id_] = update_chat_title.title_;
                         },
                         [this](td_api::updateUser &update_user)
                         {
                             auto user_id = update_user.user_->id_;
                             users_[user_id] = std::move(update_user.user_);
                         },
                         [this](td_api::updateNewMessage &update_new_message)
                         {
                             auto chat_id = update_new_message.message_->chat_id_;
                             std::string sender_name;
                             td_api::downcast_call(*update_new_message.message_->sender_id_,
                                                   overloaded(
                                                       [this, &sender_name](td_api::messageSenderUser &user)
                                                       {
                                                           sender_name = get_chat_title(user.user_id_);
                                                       },
                                                       [this, &sender_name](td_api::messageSenderChat &chat)
                                                       {
                                                           sender_name = get_chat_title(chat.chat_id_);
                                                       }));
                             std::string text;
                             if (update_new_message.message_->content_->get_id() == td_api::messageText::ID)
                             {
                                 text = static_cast<td_api::messageText &>(*update_new_message.message_->content_).text_->text_;
                             }
                             std::cout << "Receive message: [chat_id:" << chat_id << "] [from:" << sender_name << "] ["
                                       << text << "]" << std::endl;
                         },
                         
                         [](auto &update) {}));
    }

    auto create_authentication_query_handler()
    {
        return [this, id = authentication_query_id_](Object object)
        {
            if (id == authentication_query_id_)
            {
                check_authentication_error(std::move(object));
            }
        };
    }



  void on_authorization_state_ready() 
  {
    are_authorized_ = true;
    std::cout << "Authorization is completed" << std::endl;
  }

  void authorization_state_logging_out()
  {
    are_authorized_ = false;
    std::cout<< " Logging out " << std::endl;
  }
  void authorization_state_wait_phone_number()
  {

    std::cout << " Enter phone number: " << std::flush;
    std::string phone_number;
    std::cin >> phone_number;
    send_query(
        td_api::make_object<td_api::setAuthenticationPhoneNumber>(phone_number, nullptr),
        create_authentication_query_handler());
  
   }
  void authorization_state_closed()
  {
    are_authorized_ = false;
    need_restart_ = true;
    std::cout << "Terminated" << std::endl;
  }
  void authorization_state_wait_email_address(){
    std::cout << " Enter email address: " << std::flush;
    std::string email_address;
    std::cin >> email_address;
    send_query(td_api::make_object<td_api::setAuthenticationEmailAddress>(email_address),
        create_authentication_query_handler());
  }
  void authorization_state_wait_email_code(){
    std::cout << " Enter email authentication code: " << std::flush;
    std::string code;
    std::cin >> code;
    send_query(td_api::make_object<td_api::checkAuthenticationEmailCode>(
        td_api::make_object<td_api::emailAddressAuthenticationCode>(code)),
            create_authentication_query_handler());
  }
  void authorization_state_wait_code()
  {
    std::cout << " Enter authentication code: " << std::flush;
    std::string code;
    std::cin >> code;
    send_query(td_api::make_object<td_api::checkAuthenticationCode>(code),
            create_authentication_query_handler());
  }
  void authorization_state_wait_registration(){
    std::string first_name;
    std::string last_name;
    std::cout << "Enter your first name: " << std::flush;
    std::cin >> first_name;
    std::cout << "Enter your last name: " << std::flush;
    std::cin >> last_name;
    send_query(td_api::make_object<td_api::registerUser>(first_name, last_name, false),
    create_authentication_query_handler());
  }
  void authorization_state_wait_password(){
    std::cout << "Enter authentication password: " << std::flush;
    std::string password;
    std::getline(std::cin, password);
    send_query(td_api::make_object<td_api::checkAuthenticationPassword>(password),
    create_authentication_query_handler());
  }
  void on_authorization_state_update() {
    authentication_query_id_++;
    td_api::downcast_call(*authorization_state_,
                          overloaded(
                              [this](td_api::authorizationStateReady &) {
                               on_authorization_state_ready();
                              },
                              [this](td_api::authorizationStateLoggingOut &) {
                                authorization_state_logging_out();
                              },
                              [this](td_api::authorizationStateClosing &) { std::cout << "Closing" << std::endl; },
                              [this](td_api::authorizationStateClosed &) {
                                authorization_state_closed();
                              },
                              [this](td_api::authorizationStateWaitPhoneNumber &) {
                                authorization_state_wait_phone_number();
                              },
                              [this](td_api::authorizationStateWaitPremiumPurchase &) {
                                std::cout << "Telegram Premium subscription is required" << std::endl;
                              },
                              [this](td_api::authorizationStateWaitEmailAddress &) {
                                authorization_state_wait_email_address();
                              },
                              [this](td_api::authorizationStateWaitEmailCode &) {
                                authorization_state_wait_email_code();
                              },
                              [this](td_api::authorizationStateWaitCode &) {
                                authorization_state_wait_code();
                              },
                              [this](td_api::authorizationStateWaitRegistration &) {
                                authorization_state_wait_registration();
                              },
                              [this](td_api::authorizationStateWaitPassword &) {
                                authorization_state_wait_password();
                              },
                              [this](td_api::authorizationStateWaitOtherDeviceConfirmation &state) {
                                std::cout << "Confirm this login link on another device: " << state.link_ << std::endl;
                              },
                              [this](td_api::authorizationStateWaitTdlibParameters &) {
                                auto request = td_api::make_object<td_api::setTdlibParameters>();
                                request->database_directory_ = "tdlib";
                                request->use_message_database_ = true;
                                request->use_secret_chats_ = true;
                                request->api_id_ = 23832878;
                                request->api_hash_ = "*";
                                request->system_language_code_ = "en";
                                request->device_model_ = "Desktop";
                                request->application_version_ = "1.0";
                                send_query(std::move(request), create_authentication_query_handler());
                              }));
  }

    void check_authentication_error(Object object)
    {
        if (object->get_id() == td_api::error::ID)
        {
            auto error = td::move_tl_object_as<td_api::error>(object);
            std::cout << "Error:" << to_string(error) << std::flush;
            on_authorization_state_update();
        }
    }

    std::uint64_t next_query_id()
    {
        return ++current_query_id_;
    }
};

int main()
{
    TdBasic basicTg;
    basicTg.loop();
}
