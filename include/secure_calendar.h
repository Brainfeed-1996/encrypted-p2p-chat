#ifndef SECURE_CALENDAR_H
#define SECURE_CALENDAR_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <chrono>

namespace Crypto {

struct SecureEvent {
    std::string event_id;
    std::string title;
    std::string description;
    std::vector<uint8_t> encrypted_content;
    uint64_t start_time;
    uint64_t end_time;
    std::string location;
    std::string organizer_id;
    std::vector<std::string> attendee_ids;
    bool is_private;
    bool is_recurring;
    std::string recurrence_pattern;
    std::vector<std::string> reminders;
    std::vector<std::string> tags;
    uint64_t created_at;
    uint64_t modified_at;
    std::string calendar_id;
};

struct Calendar {
    std::string calendar_id;
    std::string name;
    std::string description;
    std::string owner_id;
    std::vector<std::string> shared_with;
    bool is_default;
    bool is_encrypted;
    uint64_t created_at;
    uint64_t modified_at;
    std::vector<std::string> color;
};

struct EventInvitation {
    std::string invitation_id;
    std::string event_id;
    std::string sender_id;
    std::string recipient_id;
    std::string status; // pending, accepted, declined, tentative
    uint64_t sent_at;
    uint64_t responded_at;
    std::string personal_message;
};

struct TimeSlot {
    uint64_t start_time;
    uint64_t end_time;
    bool is_available;
    double score; // availability score 0-1
};

class SecureCalendar {
public:
    SecureCalendar();
    ~SecureCalendar();
    
    bool initialize();
    
    // Calendar management
    Calendar create_calendar(const std::string& name, const std::string& owner_id);
    bool delete_calendar(const std::string& calendar_id);
    bool share_calendar(const std::string& calendar_id, const std::string& user_id, const std::string& permission);
    std::vector<Calendar> list_calendars(const std::string& user_id);
    
    // Event management
    SecureEvent create_event(const std::string& calendar_id, const std::string& title,
                            const std::string& description, uint64_t start_time, uint64_t end_time);
    bool update_event(const std::string& event_id, const SecureEvent& updates);
    bool delete_event(const std::string& event_id);
    bool invite_attendee(const std::string& event_id, const std::string& attendee_id);
    bool respond_to_invitation(const std::string& invitation_id, const std::string& response);
    
    // Event queries
    std::vector<SecureEvent> get_events(const std::string& calendar_id, uint64_t start_time, uint64_t end_time);
    SecureEvent get_event(const std::string& event_id);
    std::vector<SecureEvent> search_events(const std::string& calendar_id, const std::string& query);
    
    // Availability
    std::vector<TimeSlot> find_available_slots(const std::vector<std::string>& attendee_ids,
                                              uint64_t start_time, uint64_t end_time, uint32_t duration_minutes);
    std::vector<TimeSlot> get_busy_times(const std::string& user_id, uint64_t start_time, uint64_t end_time);
    
    // Reminders
    void set_reminder(const std::string& event_id, uint64_t reminder_time);
    void snooze_reminder(const std::string& reminder_id, uint64_t new_time);
    
    // Security
    void enable_e2e_encryption(bool enable);
    void configure_retention_policy(uint32_t days);
    void enable_private_events(bool enable);
    
    void generate_calendar_report();
    
private:
    bool initialized_;
    bool e2e_encryption_enabled_;
    uint32_t retention_days_;
    bool private_events_enabled_;
    
    std::map<std::string, Calendar> calendars_;
    std::map<std::string, SecureEvent> events_;
    std::map<std::string, EventInvitation> invitations_;
    
    std::vector<uint8_t> encrypt_event_data(const std::string& data);
    std::string decrypt_event_data(const std::vector<uint8_t>& encrypted);
    std::string generate_event_id();
    std::string generate_calendar_id();
    bool check_calendar_access(const std::string& calendar_id, const std::string& user_id);
};

} // namespace Crypto

#endif // SECURE_CALENDAR_H
