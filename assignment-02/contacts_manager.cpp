#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;

class Contact {
public:
  string first_name, last_name, phone_no, email;

  Contact(string first_name, string last_name, string phone_no, string email) {
    this->first_name = first_name;
    this->last_name = last_name;
    this->phone_no = phone_no;
    this->email = email;
  }
};

class ContactManager {
private:
  unordered_map<string, Contact> hash_table;

public:
  void add_new_contact(Contact contact);
  Contact* retrieve_contact(string first_name);
  bool delete_contact(string first_name);
  void update_contact(string first_name, Contact contact);
};

void ContactManager::add_new_contact(Contact contact) {
  hash_table.insert(make_pair(contact.first_name, contact));
  // TODO: how to determine it was successful?
}

Contact* ContactManager::retrieve_contact(string first_name) {
  unordered_map<string, Contact>::iterator result = hash_table.find(first_name);

  // If not found, find() will return a iterator pointing to the end of the container:
  if (result == hash_table.end()) { throw "Could not find contact"; }

  // Return pointer to the value of the result <key, value> pair
  return &get<1>(*result);
}

bool ContactManager::delete_contact(string first_name) {
  int removed_count = hash_table.erase(first_name);

  return removed_count > 0;
}

int main() {
  ContactManager contact_manager;
  Contact bob = Contact("Bob", "McBobson", "3473334444", "bob@bob.com");

  contact_manager.add_new_contact(bob);
  contact_manager.delete_contact("Tom");

  Contact* result = contact_manager.retrieve_contact("Bob");

  cout << (*result).first_name << endl;
  return 0;
}

