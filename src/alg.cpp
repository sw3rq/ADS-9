// Copyright 2022 NNTU-CS
#include <algorithm>
#include <vector>
#include "tree.h"

PMTree::PMTree(const std::vector<char>& src) : base(src) {
  top = new Node(0);
  std::vector<char> sorted = src;
  std::sort(sorted.begin(), sorted.end());
  for (char ch : sorted) {
    std::vector<char> leftover = sorted;
    leftover.erase(std::find(leftover.begin(), leftover.end(), ch));
    Node* child = generate(leftover);
    child->sym = ch;
    top->links.push_back(child);
  }
}

PMTree::~PMTree() {
  destroy(top);
}

PMTree::Node* PMTree::generate(const std::vector<char>& rest) {
  Node* cur = new Node(0);
  if (rest.empty()) return cur;
  std::vector<char> sorted = rest;
  std::sort(sorted.begin(), sorted.end());
  for (char ch : sorted) {
    std::vector<char> leftover = sorted;
    leftover.erase(std::find(leftover.begin(), leftover.end(), ch));
    Node* child = generate(leftover);
    child->sym = ch;
    cur->links.push_back(child);
  }
  return cur;
}

void PMTree::destroy(Node* ptr) {
  if (!ptr) return;
  for (PMTree::Node* child : ptr->links)
    destroy(child);
  delete ptr;
}

static void traverse(PMTree::Node* cur, std::vector<char>& buf,
                     std::vector<std::vector<char>>& out,
                     int level, int limit) {
  if (level == limit) {
    out.push_back(buf);
    return;
  }
  for (PMTree::Node* nxt : cur->links) {
    buf.push_back(nxt->sym);
    traverse(nxt, buf, out, level + 1, limit);
    buf.pop_back();
  }
}

std::vector<std::vector<char>> getAllPerms(PMTree& obj) {
  std::vector<std::vector<char>> out;
  std::vector<char> buf;
  int total = static_cast<int>(obj.base.size());
  for (PMTree::Node* first : obj.top->links) {
    buf.push_back(first->sym);
    traverse(first, buf, out, 1, total);
    buf.pop_back();
  }
  return out;
}

size_t fact(int n) {
  size_t res = 1;
  for (int i = 2; i <= n; ++i) res *= i;
  return res;
}

std::vector<char> getPerm1(PMTree& obj, int pos) {
  auto all = getAllPerms(obj);
  if (pos <= 0 || pos > static_cast<int>(all.size())) return {};
  return all[pos - 1];
}

std::vector<char> getPerm2(PMTree& obj, int pos) {
  int total = static_cast<int>(obj.base.size());
  if (pos <= 0 || static_cast<size_t>(pos) > fact(total)) return {};
  std::vector<char> result;
  int remainder = pos - 1;
  PMTree::Node* cur = obj.top;
  for (int step = 0; step < total; ++step) {
    int block = static_cast<int>(fact(total - step - 1));
    int idx = remainder / block;
    remainder %= block;
    if (idx >= static_cast<int>(cur->links.size())) return {};
    cur = cur->links[idx];
    result.push_back(cur->sym);
  }
  return result;
}
