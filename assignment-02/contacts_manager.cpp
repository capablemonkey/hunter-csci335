#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>

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

  void print() {
    cout << first_name << ": " << last_name << " | "  << phone_no << " | " << email << endl;
  }
};

class ContactManager {
private:
  unordered_map<string, Contact> hash_table;

public:
  void add_new_contact(Contact contact);
  Contact* retrieve_contact(string first_name);
  bool delete_contact(string first_name);
  bool update_contact(string first_name, string last_name, string phone_no, string email);
};

void ContactManager::add_new_contact(Contact contact) {
  hash_table.insert(make_pair(contact.first_name, contact));
  // TODO: how to determine it was successful?
}

class ContactNotFoundException : public exception { };

Contact* ContactManager::retrieve_contact(string first_name) {
  unordered_map<string, Contact>::iterator result = hash_table.find(first_name);

  // If not found, find() will return a iterator pointing to the end of the container:
  if (result == hash_table.end()) { throw ContactNotFoundException(); }

  // Return pointer to the value of the result <key, value> pair
  return &get<1>(*result);
}

bool ContactManager::delete_contact(string first_name) {
  int removed_count = hash_table.erase(first_name);
  return removed_count > 0;
}

bool ContactManager::update_contact(string first_name, string last_name, string phone_no, string email) {
  try {
    Contact *original = retrieve_contact(first_name);
    original->last_name = last_name;
    original->phone_no = phone_no;
    original->email = email;
    return true;
  } catch (const ContactNotFoundException) { }

  return false;
}

int main() {
  ContactManager contact_manager;
  Contact bob = Contact("Bob", "McBobson", "3473334444", "bob@bob.com");
  Contact tom = Contact("Tom", "Jaster", "6462228888", "tom@tom.com");

  // add contacts:
  contact_manager.add_new_contact(bob);
  contact_manager.add_new_contact(tom);

  // retrieve contact:
  Contact* result = contact_manager.retrieve_contact("Bob");
  result->print();

  // update contact:
  contact_manager.update_contact("Bob", "Samuels", "9991112222", "bob@samuels.com");
  result = contact_manager.retrieve_contact("Bob");
  result->print();

  // delete contact:
  bool deleted = contact_manager.delete_contact("Tom");
  if (deleted) {
    cout << "PASS: Tom was deleted" << endl;
  } else {
    cout << "FAIL: Tom didn't get deleted" << endl;
  }

  deleted = contact_manager.delete_contact("Roger");
  if (deleted) { 
    cout << "FAIL: Delete somehow worked" << endl; 
  } else { 
    cout << "PASS: Could not delete non-existent Roger" << endl; 
  }

  return 0;
}

