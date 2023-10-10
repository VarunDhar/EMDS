//NOTE: !!!! save this as "reporting_hierarchy.h" !!!!
// This header file assumes that some data is already present in Database, such as manager names, etc. 
// Otherwise 'addEmployee' and 'removeEmployee' wont work

#pragma once
#ifndef reporting_hierarchy_h
#define reporting_hierarchy_h

#include<iostream>
#include<vector>
#include<unordered_set>
#include<unordered_map>
#include<string>
#include<queue>

using namespace std;

class ReportingHierarchy {
	//one graph (for emp->manager), and one for manager->emp
	//to check loop, use dfs(finding cycle)
	//remove employee, before that check if employee has children(edges)
	//to move employee, simply remove the employee from the managers list, and add it to another managers list
// 	unordered_map<string, unordered_set<string>> manager_emp;
// 	unordered_map<string,string> emp_manager;

	bool checkCycle(string current, unordered_map<string, unordered_set<string>>& manager_emp, unordered_map<string, int>& vis, unordered_map<string, int>& dfsVis) {
		vis[current] = 1;
		dfsVis[current] = 1;
		for (auto i : manager_emp[current]) {
			if (!vis[i]) {
				if (checkCycle(i, manager_emp, vis, dfsVis)) return true;
			}
			else if (dfsVis[i]) {
				return true;
			}
		}
		dfsVis[current] = 0;
		return false;
	}

public:
	unordered_map<string, unordered_set<string>> manager_emp;
	unordered_map<string, string> emp_manager;
	ReportingHierarchy() { cout << "Object created!\n"; }

	void printDirectReports(string manager) {
		if (manager_emp.find(manager) == manager_emp.end()) {
			cout << "Error:Manager not found!";
			cout << "\n-x-x-\n";

			return;
		}
		else {
			cout << "Employees reporting to :" << manager << " are =>\n";
			for (auto i : manager_emp[manager]) {
				cout << (i) << "\n";
			}

			cout << "\n-x-x-\n";

			return;
		}
	}
	void printSubOrg(string manager) {
		//We can use Breadth First search for this problem
		if (manager_emp.find(manager) == manager_emp.end()) {
			cout << "ERROR: " << manager << " not found in DB!\n";
			cout << "\n-x-x-\n";
			return;
		}
		unordered_set<string> vis;
		vis.insert(manager);
		queue<string> q;
		q.push(manager);
		cout << "Full suborganization reporting to : " << manager << " is =>\n";
		while (!q.empty()) {
			string emp = q.front();
			q.pop();
			cout << emp << " ";
			for (auto i : manager_emp[emp]) {
				if (vis.find(i) == vis.end())
				{
					q.push(i);
					vis.insert(i);
				}
			}
		}

		cout << "\n-x-x-\n";

		return;
	}
	void printManagementChain(string employee) {
		//We can use Breadth First search for this problem
		if (emp_manager.find(employee) == emp_manager.end()) {
			cout << "ERROR: " << employee << " not found in DB!\n";
			cout << "\n-x-x-\n";

			return;
		}
		unordered_set<string> vis;
		vis.insert(employee);
		queue<string> q;
		q.push(employee);
		vector<string> ans;

		cout << "Management chain of : " << employee << " is =>\n";

		while (!q.empty()) {
			string emp = q.front();
			q.pop();
			ans.push_back(emp);
			if (vis.find(emp_manager[emp]) == vis.end())
			{
				q.push(emp_manager[emp]);
				vis.insert(emp_manager[emp]);
			}
		}

		//printing in management chain of the employee
		for (auto i = ans.begin(); i != ans.end(); i++) {
			cout << " " << *i;
		}
		cout << "\n-x-x-\n";


		return;
	}

	bool checkLoop() {
		//we can use Depth first search (DFS) for this problem
		unordered_map<string, int> vis, dfsVis;

		for (auto i : manager_emp) {
			vis[i.first] = 0;
			dfsVis[i.first] = 0;
		}

		for (auto i : manager_emp) {
			if (!vis[i.first]) {
				string curr = i.first;
				if (checkCycle(i.first, manager_emp, vis, dfsVis)) {
					return true;
				}
			}
		}
		return false;
	}

	void addEmployee(string emp, string manager) {
		if (manager_emp.find(manager) != manager_emp.end() && emp_manager.find(emp) == emp_manager.end()) {
			manager_emp[emp];
			manager_emp[manager].insert(emp);
			emp_manager[emp] = manager;
			if (checkLoop()) {
				manager_emp.erase(emp);
				emp_manager.erase(emp);
				manager_emp[manager].erase(emp);
				cout << "Adding " << emp << " creating loop";

				cout << "\n-x-x-\n";

				return;
			}
			else {
				cout << emp << " added successfully to : " << manager;

				cout << "\n-x-x-\n";

				return;
			}
		}
		cout << "ERROR:Employee already exists or Manager doesn't exist\n";

		cout << "\n-x-x-\n";

		return;

	}

	void removeEmployee(string emp) {
		if (manager_emp.find(emp) == manager_emp.end()) {
			cout << "ERROR: " << emp << " doesn't exist in DB\n";

			cout << "\n-x-x-\n";

			return;
		}
		if (manager_emp[emp].size() == 0) {
			manager_emp.erase(emp);
			manager_emp[emp_manager[emp]].erase(emp); // remove emp from the list of employees reporting to its manager
			emp_manager.erase(emp);
			cout << emp << " successfully removed from DB\n";
			cout << "\n-x-x-\n";

			return;
		}
		cout << "ERROR: " << emp << " removal will create broken/disconnected chain\n";

		cout << "\n-x-x-\n";

		return;
	}

	void moveEmployee(string emp, string new_manager) {
		if (emp_manager.find(emp) != emp_manager.end() && manager_emp.find(new_manager) != manager_emp.end()) {
			//cout << "ERROR: " << emp << " doesn't exist in DB\n";
			manager_emp[emp_manager[emp]].erase(emp);
			manager_emp[new_manager].insert(emp);
			emp_manager[emp] = new_manager;

			cout << "SUCCESS: " << emp << "'s manager changed to : " << new_manager << "\n";
			cout << "\n-x-x-\n";

			return;
		}
		cout << "ERROR: " << emp << "'s manager couldn't be changed\n";
		cout << "\n-x-x-\n";

		return;

	}
};

#endif
