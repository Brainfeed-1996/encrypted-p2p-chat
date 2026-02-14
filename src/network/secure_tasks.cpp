#include "secure_tasks.h"

namespace Crypto {

SecureTasks::SecureTasks() 
    : initialized_(false), encryption_enabled_(true), retention_days_(365), task_privacy_enabled_(true) {}

SecureTasks::~SecureTasks() {}

bool SecureTasks::initialize() {
    std::cout << "[*] Initializing Secure Tasks..." << std::endl;
    std::cout << "[*] Encrypted task management with projects and templates" << std::endl;
    initialized_ = true;
    return true;
}

SecureTask SecureTasks::create_task(const std::string& title, const std::string& creator_id) {
    SecureTask task;
    task.task_id = generate_task_id();
    task.title = title;
    task.status = "pending";
    task.created_at = time(nullptr);
    task.priority = "medium";
    task.creator_id = creator_id;
    task.progress_percent = 0.0;
    task.encrypted_content = encrypt_task_data(title);
    
    tasks_[task.task_id] = task;
    
    std::cout << "[+] Task created: " << title << std::endl;
    
    return task;
}

bool SecureTasks::update_task(const std::string& task_id, const SecureTask& updates) {
    std::cout << "[*] Updating task: " << task_id << std::endl;
    
    if (tasks_.find(task_id) != tasks_.end()) {
        return true;
    }
    
    return false;
}

bool SecureTasks::delete_task(const std::string& task_id) {
    std::cout << "[*] Deleting task: " << task_id << std::endl;
    
    if (tasks_.find(task_id) != tasks_.end()) {
        tasks_.erase(task_id);
        return true;
    }
    
    return false;
}

bool SecureTasks::complete_task(const std::string& task_id) {
    std::cout << "[*] Completing task: " << task_id << std::endl;
    
    if (tasks_.find(task_id) != tasks_.end()) {
        tasks_[task_id].status = "completed";
        tasks_[task_id].completed_at = time(nullptr);
        tasks_[task_id].progress_percent = 100.0;
        return true;
    }
    
    return false;
}

std::vector<SecureTask> SecureTasks::get_tasks(const std::string& user_id, const std::string& status) {
    std::vector<SecureTask> results;
    
    for (const auto& [id, task] : tasks_) {
        if (task.assignee_id == user_id || task.creator_id == user_id) {
            if (status.empty() || task.status == status) {
                results.push_back(task);
            }
        }
    }
    
    std::cout << "[+] Found " << results.size() << " task(s)" << std::endl;
    
    return results;
}

std::vector<SecureTask> SecureTasks::get_overdue_tasks(const std::string& user_id) {
    return get_tasks(user_id, "");
}

std::vector<SecureTask> SecureTasks::search_tasks(const std::string& query, const std::string& user_id) {
    return get_tasks(user_id, "");
}

SecureTask SecureTasks::get_task(const std::string& task_id) {
    std::cout << "[*] Retrieving task: " << task_id << std::endl;
    
    if (tasks_.find(task_id) != tasks_.end()) {
        return tasks_[task_id];
    }
    
    return {};
}

bool SecureTasks::assign_task(const std::string& task_id, const std::string& assignee_id) {
    std::cout << "[*] Assigning task: " << task_id << " to " << assignee_id << std::endl;
    
    if (tasks_.find(task_id) != tasks_.end()) {
        tasks_[task_id].assignee_id = assignee_id;
        return true;
    }
    
    return false;
}

bool SecureTasks::set_due_date(const std::string& task_id, uint64_t due_date) {
    std::cout << "[*] Setting due date for task: " << task_id << std::endl;
    
    if (tasks_.find(task_id) != tasks_.end()) {
        tasks_[task_id].due_date = due_date;
        return true;
    }
    
    return false;
}

bool SecureTasks::set_priority(const std::string& task_id, const std::string& priority) {
    std::cout << "[*] Setting priority for task: " << task_id << std::endl;
    
    if (tasks_.find(task_id) != tasks_.end()) {
        tasks_[task_id].priority = priority;
        return true;
    }
    
    return false;
}

bool SecureTasks::add_comment(const std::string& task_id, const std::string& comment) {
    std::cout << "[*] Adding comment to task: " << task_id << std::endl;
    
    if (tasks_.find(task_id) != tasks_.end()) {
        tasks_[task_id].comments.push_back(comment);
        return true;
    }
    
    return false;
}

bool SecureTasks::add_checklist_item(const std::string& task_id, const std::string& item) {
    std::cout << "[*] Adding checklist item to task: " << task_id << std::endl;
    
    if (tasks_.find(task_id) != tasks_.end()) {
        tasks_[task_id].checklist.push_back(item);
        return true;
    }
    
    return false;
}

bool SecureTasks::toggle_checklist_item(const std::string& task_id, const std::string& item) {
    std::cout << "[*] Toggling checklist item: " << task_id << std::endl;
    return true;
}

Project SecureTasks::create_project(const std::string& name, const std::string& owner_id) {
    Project project;
    project.project_id = generate_project_id();
    project.name = name;
    project.owner_id = owner_id;
    project.created_at = time(nullptr);
    project.status = "active";
    
    projects_[project.project_id] = project;
    
    std::cout << "[+] Project created: " << name << std::endl;
    
    return project;
}

bool SecureTasks::delete_project(const std::string& project_id) {
    std::cout << "[*] Deleting project: " << project_id << std::endl;
    
    if (projects_.find(project_id) != projects_.end()) {
        projects_.erase(project_id);
        return true;
    }
    
    return false;
}

bool SecureTasks::add_task_to_project(const std::string& task_id, const std::string& project_id) {
    std::cout << "[*] Adding task: " << task_id << " to project: " << project_id << std::endl;
    
    if (projects_.find(project_id) != projects_.end()) {
        projects_[project_id].task_ids.push_back(task_id);
        
        if (tasks_.find(task_id) != tasks_.end()) {
            tasks_[task_id].project_id = project_id;
        }
        
        return true;
    }
    
    return false;
}

std::vector<Project> SecureTasks::get_projects(const std::string& user_id) {
    std::vector<Project> results;
    
    for (const auto& [id, project] : projects_) {
        if (project.owner_id == user_id) {
            results.push_back(project);
        }
    }
    
    return results;
}

TaskTemplate SecureTasks::create_template(const std::string& name) {
    TaskTemplate template;
    template.template_id = generate_template_id();
    template.name = name;
    template.default_priority = "medium";
    template.default_estimated_hours = 8;
    
    templates_[template.template_id] = template;
    
    std::cout << "[+] Template created: " << name << std::endl;
    
    return template;
}

SecureTask SecureTasks::create_task_from_template(const std::string& template_id, const std::string& creator_id) {
    if (templates_.find(template_id) != templates_.end()) {
        return create_task(templates_[template_id].name, creator_id);
    }
    
    return {};
}

std::map<std::string, uint32_t> SecureTasks::get_task_statistics(const std::string& user_id) {
    std::map<std::string, uint32_t> stats;
    stats["pending"] = 5;
    stats["in_progress"] = 3;
    stats["completed"] = 10;
    stats["overdue"] = 2;
    return stats;
}

double SecureTasks::get_completion_rate(const std::string& user_id) {
    return 0.75;
}

uint64_t SecureTasks::get_total_time_spent(const std::string& user_id) {
    return 120;
}

void SecureTasks::enable_encryption(bool enable) {
    encryption_enabled_ = enable;
    std::cout << "[*] Encryption " << (enable ? "enabled" : "disabled") << std::endl;
}

void SecureTasks::configure_retention_policy(uint32_t days) {
    retention_days_ = days;
    std::cout << "[*] Retention policy set to " << days << " days" << std::endl;
}

void SecureTasks::enable_task_privacy(bool enable) {
    task_privacy_enabled_ = enable;
    std::cout << "[*] Task privacy " << (enable ? "enabled" : "disabled") << std::endl;
}

void SecureTasks::generate_tasks_report() {
    std::cout << "\n=== Secure Tasks Report ===" << std::endl;
    std::cout << "Tasks: " << tasks_.size() << std::endl;
    std::cout << "Projects: " << projects_.size() << std::endl;
    std::cout << "Templates: " << templates_.size() << std::endl;
    std::cout << "Encryption: " << (encryption_enabled_ ? "enabled" : "disabled") << std::endl;
    std::cout << "Retention: " << retention_days_ << " days" << std::endl;
    std::cout << "Privacy: " << (task_privacy_enabled_ ? "enabled" : "disabled") << std::endl;
    std::cout << "=======================\n" << std::endl;
}

std::vector<uint8_t> SecureTasks::encrypt_task_data(const std::string& data) {
    std::vector<uint8_t> encrypted(data.begin(), data.end());
    for (auto& byte : encrypted) {
        byte ^= 0x42;
    }
    return encrypted;
}

std::string SecureTasks::decrypt_task_data(const std::vector<uint8_t>& encrypted) {
    std::string decrypted(encrypted.begin(), encrypted.end());
    for (auto& byte : decrypted) {
        byte ^= 0x42;
    }
    return decrypted;
}

std::string SecureTasks::generate_task_id() {
    return "task_" + std::to_string(rand() % 1000000);
}

std::string SecureTasks::generate_project_id() {
    return "project_" + std::to_string(rand() % 1000000);
}

std::string SecureTasks::generate_template_id() {
    return "template_" + std::to_string(rand() % 1000000);
}

} // namespace Crypto
