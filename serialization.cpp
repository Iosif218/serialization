#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct ListNode { 
    ListNode* prev = nullptr;
    ListNode* next = nullptr;
    ListNode* rand = nullptr;
    std::string data;
};

int main() {
    std::ifstream in("inlet.in");
    if (!in) {
        std::cerr << "Не удалось открыть inlet.in\n";
        return 1;
    }

    std::vector<ListNode*> nodes;
    std::vector<int> rand_indices;
    std::string line;

    while (std::getline(in, line)) {
        size_t pos = line.rfind(';'); 
        if (pos == std::string::npos) {
            std::cerr << "Некорректная строка: " << line << "\n";
            return 1;
        }

        std::string data = line.substr(0, pos);
        int rand_index = std::stoi(line.substr(pos + 1));

        ListNode* node = new ListNode();
        node->data = data;

        nodes.push_back(node);
        rand_indices.push_back(rand_index);
    }

    for (size_t i = 0; i < nodes.size(); ++i) {
        if (i > 0) {
            nodes[i]->prev = nodes[i - 1];
        }
        if (i + 1 < nodes.size()) {
            nodes[i]->next = nodes[i + 1];
        }
    }

    for (size_t i = 0; i < nodes.size(); ++i) {
        int ri = rand_indices[i];
        if (ri == -1) {
            nodes[i]->rand = nullptr;
        }
        else if (ri >= 0 && ri < (int)nodes.size()) {
            nodes[i]->rand = nodes[ri];
        }
        else {
            std::cerr << "Некорректный rand_index в строке " << i << "\n";
            return 1;
        }
    }

    std::ofstream out("outlet.out", std::ios::binary);
    if (!out) {
        std::cerr << "Не удалось открыть outlet.out\n";
        return 1;
    }

    size_t count = nodes.size();
    out.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (size_t i = 0; i < nodes.size(); ++i) {
        const std::string& s = nodes[i]->data;
        size_t len = s.size();

        out.write(reinterpret_cast<const char*>(&len), sizeof(len));
        out.write(s.data(), len);

        int ri = rand_indices[i];
        out.write(reinterpret_cast<const char*>(&ri), sizeof(ri));
    }

    for (ListNode* node : nodes) {
        delete node;
    }

    return 0;
}