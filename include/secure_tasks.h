#ifndef SECURE_TASKS_H
#define SECURE_TASKS_H

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>
#include <chrono>

namespace Crypto {

struct SecureTask {
    std::string task_id;
    std::string title;
    std::string description;
    std::vector<uint8_t> encrypted_content;
    std::string status; // pending, in_progress, completed, cancelled
    uint64_t created_at;
    uint64_t due_date;
    uint64_t completed_at;
    uint64_t reminder_time;
    std::string priority; // low, medium, high, urgent
    std::string category;
    std::vector<std::string> tags;
    std::string project_id;
    std::string assignee_id;
    std::string creator_id;
    std::vector<std::string> checklist;
    std::vector<std::string> comments;
    std::vector<std::string> attachments;
    uint64_t estimated_hours;
    uint64_t actual_hours;
    double progress_percent;
};

struct Project {
    std::string project_id;
    std::string name;
    std::string description;
    std::string owner_id;
    std::vector<std::string> members;
    std::vector<std::string> task_ids;
    uint64_t created_at;
    uint64_t deadline;
    std::string status; // active, completed, archived
    std::vector<std::string> milestones;
};

struct TaskTemplate {
    std::string template_id;
    std::string name;
    std::string description;
    std::vector<std::string> default_tags;
    std::vector<std::string> checklist_items;
    std::string default_priority;
    uint64_t default_estimated_hours;
};

class SecureTasks {
public:
    SecureTasks();
    ~SecureTasks();
    
    bool initialize();
    
    // Task management
    SecureTask create_task(const std::string& title, const std::string& creator_id);
    bool update_task(const std::string& task_id, const SecureTask& updates);
    bool delete_task(const std::string& task_id);
    bool complete_task(const std::string& task_id);
    
    // Task queries
    std::vector<SecureTask> get_tasks(const std::string& user_id, const std::string& status);
    std::vector<SecureTask> get_overdue_tasks(const std::string& user_id);
    std::vector<SecureTask> search_tasks(const std::string& query, const std::string& user_id);
    SecureTask get_task(const std::string& task_id);
    
    // Task operations
    bool assign_task(const std::string& task_id, const std::string& assignee_id);
    bool set_due_date(const std::string& task_id, uint64_t due_date);
    bool set_priority(const std::string& task_id, const std::string& priority);
    bool add_comment(const std::string& task_id, const std::string& comment);
    bool add_checklist_item(const std::string& task_id, const std::string& item);
    bool toggle_checklist_item(const std::string& task_id, const std::string& item);
    
    // Project management
    Project create_project(const std::string& name, const std::string& owner_id);
    bool delete_project(const std::string& project_id);
    bool add_task_to_project(const std::string& task_id, const std::string& project_id);
    std::vector<Project> get_projects(const std::string& user_id);
    
    // Templates
    TaskTemplate create_template(const std::string& name);
    SecureTask create_task_from_template(const std::string& template_id, const std::string& creator_id);
    
    // Statistics
    std::map<std::string, uint32_t> get_task_statistics(const std::string& user_id);
    double get_completion_rate(const std::string& user_id);
    uint64_t get_total_time_spent(const std::string& user_id);
    
    // Security
    void enable_encryption(bool enable);
    void configure_retention_policy(uint32_t days);
    void enable_task_privacy(bool enable);
    
    void generate_tasks_report();
    
private:
    bool initialized_;
    bool encryption_enabled_;
    uint32_t retention_days_;
    bool task_privacy_enabled_;
    
    std::map<std::string, SecureTask> tasks_;
    std::map<std::string, Project> projects_;
    std::map<std::string, TaskTemplate> templates_;
    
    std::vector<uint8_t> encrypt_task_data(const std::string& data);
    std::string decrypt_task_data(const std::vector<uint8_t>& encrypted);
    std::string generate_task_id();
    std::string generate_project_id();
    std::string generate_template_id();
};

} // namespace Crypto

#endif // SECURE_TASKS_H
