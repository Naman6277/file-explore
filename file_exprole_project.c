#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_NAME_LENGTH 50

typedef struct TreeNode {
    char name[MAX_NAME_LENGTH];
    bool isFile;
    struct TreeNode *firstChild;
    struct TreeNode *nextSibling;
} TreeNode;

TreeNode* createNode(const char *name, bool isFile) {
    TreeNode *newNode = (TreeNode*)malloc(sizeof(TreeNode));
    strncpy(newNode->name, name, MAX_NAME_LENGTH - 1);
    newNode->name[MAX_NAME_LENGTH - 1] = '\0';
    newNode->isFile = isFile;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    return newNode;
}

void addChild(TreeNode *parent, TreeNode *child) {
    if (parent == NULL) return;
    
    if (parent->firstChild == NULL) {
        parent->firstChild = child;
    } else {
        TreeNode *temp = parent->firstChild;
        while (temp->nextSibling != NULL) {
            temp = temp->nextSibling;
        }
        temp->nextSibling = child;
    }
}

void displayTree(TreeNode *node, int depth) {
    if (node == NULL) return;

    for(int i = 0; i < depth; i++) {
        printf("¦   ");
    }

    if (depth > 0) {
        printf("+-- ");
    }

    if (node->isFile) {
        printf("%s (File)\n", node->name);
    } else {
        printf("%s (Directory)\n", node->name);
        TreeNode *child = node->firstChild;
        while (child != NULL) {
            displayTree(child, depth + 1);
            child = child->nextSibling;
        }
    }
}

TreeNode* findNode(TreeNode *current, const char *name) {
    if (current == NULL) return NULL;
    
    if (strcmp(current->name, name) == 0) {
        return current;
    }

    TreeNode *found = findNode(current->firstChild, name);
    if (found != NULL) return found;

    return findNode(current->nextSibling, name);
}

void freeTree(TreeNode *root) {
    if (root == NULL) return;
    freeTree(root->firstChild);
    freeTree(root->nextSibling);
    free(root);
}

void printMenu() {
    printf("\nFile Explorer Menu:\n");
    printf("1. Create Directory\n");
    printf("2. Create File\n");
    printf("3. List Contents\n");
    printf("4. Search\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

void toLowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int main() {
    TreeNode *root = createNode("Root", false);
    TreeNode *currentDir = root;
    int choice;
    char name[MAX_NAME_LENGTH];
    char parentName[MAX_NAME_LENGTH];

    printf("=== Simple File Explorer ===\n");

    while (1) {
        printMenu();
        scanf("%d", &choice);
        getchar(); // Clear newline from input buffer

        switch (choice) {
            case 1: // Create Directory
                printf("Enter directory name: ");
                fgets(name, MAX_NAME_LENGTH, stdin);
                name[strcspn(name, "\n")] = '\0'; // Remove newline
                
                printf("Enter parent directory name (or 'current' for current directory): ");
                fgets(parentName, MAX_NAME_LENGTH, stdin);
                parentName[strcspn(parentName, "\n")] = '\0';
                
                toLowercase(parentName);
                
                TreeNode *parent;
                if (strcmp(parentName, "current") == 0) {
                    parent = currentDir;
                } else {
                    parent = findNode(root, parentName);
                }
                
                if (parent == NULL) {
                    printf("Parent directory not found!\n");
                } else if (parent->isFile) {
                    printf("Cannot add to a file!\n");
                } else {
                    TreeNode *newDir = createNode(name, false);
                    addChild(parent, newDir);
                    printf("Directory '%s' created successfully!\n", name);
                }
                break;

            case 2: // Create File
                printf("Enter file name: ");
                fgets(name, MAX_NAME_LENGTH, stdin);
                name[strcspn(name, "\n")] = '\0';
                
                printf("Enter parent directory name (or 'current' for current directory): ");
                fgets(parentName, MAX_NAME_LENGTH, stdin);
                parentName[strcspn(parentName, "\n")] = '\0';
                
                toLowercase(parentName);
                
                if (strcmp(parentName, "current") == 0) {
                    parent = currentDir;
                } else {
                    parent = findNode(root, parentName);
                }
                
                if (parent == NULL) {
                    printf("Parent directory not found!\n");
                } else if (parent->isFile) {
                    printf("Cannot add to a file!\n");
                } else {
                    TreeNode *newFile = createNode(name, true);
                    addChild(parent, newFile);
                    printf("File '%s' created successfully!\n", name);
                }
                break;

            case 3: // List Contents
                printf("\nCurrent File System Structure:\n");
                displayTree(root, 0);
                break;

            case 4: // Search
                printf("Enter name to search: ");
                fgets(name, MAX_NAME_LENGTH, stdin);
                name[strcspn(name, "\n")] = '\0';
                
                TreeNode *found = findNode(root, name);
                if (found != NULL) {
                    printf("Found: %s (%s)\n", name, found->isFile ? "File" : "Directory");
                } else {
                    printf("'%s' not found.\n", name);
                }
                break;

            case 5: // Exit
                freeTree(root);
                printf("Exiting File Explorer. Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
