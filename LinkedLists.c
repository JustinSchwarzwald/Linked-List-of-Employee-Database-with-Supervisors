
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable: 4996) // for Visual Studio

#define MAX_NAME 30

// global linked list 'list' contains the list of employees
struct employeeList {
	struct employee* employee;
	struct employeeList* next;
} *list = NULL;				// currently empty list

// structure "employee" contains the employee's name, room number and linked list of supervisors
struct employee {
	char name[MAX_NAME];
	unsigned int roomNumber;
	struct supervisor* supervisors;		// linked list 'supervisors' contains names of supervisors
};

//  structure 'supervisor' contains supervisor's name 
struct supervisor {
	char name[MAX_NAME];
	struct supervisor* next;
};

// forward declaration of functions (already implmented)
void flushStdIn();
void executeAction(char);


void addEmployee(char* employeeNameInput, unsigned int roomNumInput);
void displayEmployeeList(struct employeeList* tempList);
struct employee* searchEmployee(char* employeeNameInput);	   

void addSupervisor(char* employeeNameInput, char* supervisorNameInput);
char* lastSupervisor(char* employeeNameInput);
void removeEmployee(char* employeeNameInput);

int main()
{
	char selection = 'a';		// initialized to a dummy value
	do
	{
		printf("\nCSE240 HW 7,8\n");
		printf("Please enter your selection:\n");
		printf("HW7:\n");
		printf("\t a: add a new employee to the list\n");
		printf("\t d: display employee list (no supervisors)\n");
		printf("\t b: search for an employee on the list\n");
		printf("\t q: quit\n");
		printf("HW8:\n");
		printf("\t c: add a supervisor of a employee\n");
		printf("\t l: display last added supervisor of a employee\n");
		printf("\t r: remove a employee\n");
		printf("\t q: quit\n");

		selection = getchar();
		flushStdIn();
		executeAction(selection);
	} while (selection != 'q');

	return 0;
}

// flush out leftover '\n' characters
void flushStdIn()
{
	char c;
	do c = getchar();
	while (c != '\n' && c != EOF);
}


void executeAction(char c)
{
	char employeeNameInput[MAX_NAME], supervisorNameInput[MAX_NAME];
	unsigned int roomNumInput;
	struct employee* searchResult = NULL;

	switch (c)
	{
	case 'a':	// add employee
				// input employee details from user
		printf("\nPlease enter employee's name: ");
		fgets(employeeNameInput, sizeof(employeeNameInput), stdin);
		employeeNameInput[strlen(employeeNameInput) - 1] = '\0';	// discard the trailing '\n' char
		printf("Please enter room number: ");
		scanf("%d", &roomNumInput);
		flushStdIn();

		if (searchEmployee(employeeNameInput) == NULL)					
		{
			addEmployee(employeeNameInput, roomNumInput);
			printf("\nEmployee successfully added to the list!\n");
		}
		else
			printf("\nThat employee is already on the list!\n");
		break;

	case 'd':		// display the list
		displayEmployeeList(list);
		break;

	case 'b':		// search for an employee on the list
		printf("\nPlease enter employee's name: ");
		fgets(employeeNameInput, sizeof(employeeNameInput), stdin);
		employeeNameInput[strlen(employeeNameInput) - 1] = '\0';	// discard the trailing '\n' char

		if (searchEmployee(employeeNameInput) == NULL)					
			printf("\nEmployee name does not exist or the list is empty! \n\n");
		else
		{
			printf("\nEmployee name exists on the list! \n\n");
		}
		break;

	case 'r':		// remove employee
		printf("\nPlease enter employee's name: ");
		fgets(employeeNameInput, sizeof(employeeNameInput), stdin);
		employeeNameInput[strlen(employeeNameInput) - 1] = '\0';	// discard the trailing '\n' char

		if (searchEmployee(employeeNameInput) == NULL)			
			printf("\nEmployee name does not exist or the list is empty! \n\n");
		else
		{
			removeEmployee(employeeNameInput);
			printf("\nEmployee successfully removed from the list! \n\n");
		}
		break;

	case 'c':		// add supervisor
		printf("\nPlease enter employee's name: ");
		fgets(employeeNameInput, sizeof(employeeNameInput), stdin);
		employeeNameInput[strlen(employeeNameInput) - 1] = '\0';

		if (searchEmployee(employeeNameInput) == NULL)			
			printf("\nEmployee name does not exist or the list is empty! \n\n");
		else
		{
			printf("\nPlease enter supervisor's name: ");
			fgets(supervisorNameInput, sizeof(supervisorNameInput), stdin);
			supervisorNameInput[strlen(supervisorNameInput) - 1] = '\0';	// discard the trailing '\n' char

			addSupervisor(employeeNameInput, supervisorNameInput);
			printf("\nSupervisor added! \n\n");
		}
		break;

	case 'l':		// last supervisor
		printf("\nPlease enter employee's name: ");
		fgets(employeeNameInput, sizeof(employeeNameInput), stdin);
		employeeNameInput[strlen(employeeNameInput) - 1] = '\0';	// discard the trailing '\n' char

		if (searchEmployee(employeeNameInput) == NULL)				
			printf("\nEmployee name does not exist or the list is empty! \n\n");
		else
		{
			printf("\nLast supervisor added: %s\n\n", lastSupervisor(employeeNameInput));
		}
		break;

	case 'q':		// quit
		break;

	default: printf("%c is invalid input!\n", c);
	}
}

// Adds an employee to the list
void addEmployee(char* employeeNameInput, unsigned int roomNumInput)
{
	
	struct employeeList* newListNode; // Creates a pointer for new node for employeeList
	newListNode = (struct employeeList*) malloc(sizeof(struct employeeList)); // Allocates memory for the new node

	newListNode->employee = (struct employee*) malloc(sizeof(struct employee)); // Allocates memory for employee node under employeelist node

	strcpy(newListNode->employee->name, employeeNameInput); // Copy name into employee node
	newListNode->employee->roomNumber = roomNumInput; // Copy roomNumber into employee node
	newListNode->employee->supervisors = NULL; // Set supervisor to NULL

	if (list == NULL) // If no employees in list
	{
		newListNode->next = NULL; // Set next to null 
		list = newListNode; // Set head to the node
	}
	else
	{
		newListNode->next = list; // Set next to current head
		list = newListNode; // Set head to the new node
	}
		
}

// Displays all employees
void displayEmployeeList(struct employeeList* tempList)
{
	if (tempList == NULL) // Checks if list is empty
	{
		printf("\n(end of list)\n"); // Prints out that the list is empty and exits function
		return;
	}
	else
	{
		printf("\nEmployee name: %s\nRoom number: %d\n", tempList->employee->name, tempList->employee->roomNumber); // Prints out the current nodes info
		displayEmployeeList(tempList->next); // Recursive call using the next node in the list
	}
}

// Checks to see if an employee is on the list
struct employee* searchEmployee(char* employeeNameInput)
{
	if (list == NULL) // Checks if list is empty
		return NULL;

	struct employeeList* currentEmployee = list; // Creates a tempPointer from list
	
	while (currentEmployee != NULL) // Checks if current node is NULL
	{
		if (strcmp(currentEmployee->employee->name, employeeNameInput) == 0) // Checks if name in current node matches the name given
			return currentEmployee->employee; // Returns the pointer to the employee node with the matching name

		currentEmployee = currentEmployee->next; // Updates current node to next node
	}
	return NULL;	// edit this line as needed
}


// Adds a supervisor to an employee
void addSupervisor(char* employeeNameInput, char* supervisorNameInput)
{

	struct employeeList* tempList = list;		// work on a copy of 'list'

	
	struct employee* currentEmployee = searchEmployee(employeeNameInput); // Finds the employee node needing to add supervisor

	if (currentEmployee->supervisors == NULL) // If no current supervisors
	{
		currentEmployee->supervisors = (struct supervisor*) malloc(sizeof(struct supervisor)); // Allocating memory for the supervisor
		strcpy(currentEmployee->supervisors->name, supervisorNameInput); // Adding Name to supervisor node
		currentEmployee->supervisors->next = NULL; // Setting next to null
	}
	else // If node already has a supervisor
	{
		struct supervisor* currentSupervisor = currentEmployee->supervisors; // points to current supervisor

		while (currentSupervisor->next != NULL) // Setting current supervisor to tail of list
			currentSupervisor = currentSupervisor->next;

		currentSupervisor->next = (struct supervisor*) malloc(sizeof(struct supervisor)); // Allocating memory for the new supervisor to be added
		strcpy(currentSupervisor->next->name, supervisorNameInput); // Adding Name to new supervisor node
		currentSupervisor->next->next = NULL; // Setting next to null
	}


}

// Returns the last supervisor added to the employee
char* lastSupervisor(char* employeeNameInput)
{

	struct employeeList* tempList = list;		// work on a copy of 'list'
	// enter code here

	struct employee* currentEmployee = searchEmployee(employeeNameInput); // Finds the employee node needing to locate supervisor

	struct supervisor* currentSupervisor = currentEmployee->supervisors;
	if (currentSupervisor == NULL) // Returns string if there are no supervisors added
	{
		char* zeroSupers = "No supervisors have been added yet";
		return zeroSupers;
	}
	while (currentSupervisor->next != NULL) // Setting current supervisor to tail of list
		currentSupervisor = currentSupervisor->next;


	return currentSupervisor->name;		// edit this line as needed
}

// Removes an employee from the list, frees all supervisors and the employee
void removeEmployee(char* employeeNameInput)
{

	struct employeeList* tempList = list;	// work on a copy of 'list'

	if (tempList->next == NULL) // If only one employee node in list
	{
		struct supervisor* currentSupervisor = tempList->employee->supervisors; // Sets current supervisor to the first supervisor in list
		struct supervisor* nextSupervisor = NULL; // Defaults to NULL

		while (currentSupervisor != NULL) // Continues to remove superviosr nodes until all supervisors are deleted
		{
			if(currentSupervisor->next != NULL) // Checks if there is more than one node
				nextSupervisor = currentSupervisor->next; // Sets next node to next supervisor
			
			free(currentSupervisor); // Deletes the current first supervisor node from memory

			currentSupervisor = nextSupervisor; // Will be null if there is no next, else it becomes next supervisor node
			nextSupervisor = NULL; // Resets pointer to null
		}

		free(tempList->employee); // Free Employee node
		free(tempList); // Free employeeList node
		list = NULL; // Sets list to NULL
	}

	else // If there is more than one employee node in list
	{
		if (strcmp(tempList->employee->name, employeeNameInput) == 0) // Employee is first one in list
		{

			struct supervisor* currentSupervisor = tempList->employee->supervisors; // Sets current supervisor to the first supervisor in list
			struct supervisor* nextSupervisor = NULL; // Defaults to NULL

			while (currentSupervisor != NULL) // Continues to remove superviosr nodes until all supervisors are deleted
			{
				if (currentSupervisor->next != NULL) // Checks if there is more than one node
					nextSupervisor = currentSupervisor->next; // Sets next node to next supervisor

				free(currentSupervisor); // Deletes the current first supervisor node from memory

				currentSupervisor = nextSupervisor; // Will be null if there is no next, else it becomes next supervisor node
				nextSupervisor = NULL; // Resets pointer to null
			}

			list = list->next; // Moves head to next employee
			free(tempList->employee); // Frees employee data
			free(tempList);// Frees the employeeList Node
		}

		else
		{
			while (strcmp(tempList->next->employee->name, employeeNameInput) != 0) // Finds node before one needing to be removed
				tempList = tempList->next;

			struct employeeList* employeeToBeDeleted = tempList->next;
			struct employeeList* previousEmployee = tempList;


			previousEmployee->next = employeeToBeDeleted->next; // Assigning previous node of current employee to the following next node of current employee

			struct supervisor* currentSupervisor = employeeToBeDeleted->employee->supervisors; // Deleting employee node
			struct supervisor* nextSupervisor = NULL;

			while (currentSupervisor != NULL) // Continues to remove superviosr nodes until all supervisors are deleted
			{
				if (currentSupervisor->next != NULL) // Checks if there is more than one node
					nextSupervisor = currentSupervisor->next; // Sets next node to next supervisor

				free(currentSupervisor); // Deletes the current first supervisor node from memory

				currentSupervisor = nextSupervisor; // Will be null if there is no next, else it becomes next supervisor node
				nextSupervisor = NULL; // Resets pointer to null
			}

			free(employeeToBeDeleted->employee);
			free(employeeToBeDeleted);
		}
	}
}


