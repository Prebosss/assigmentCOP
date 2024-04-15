#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEN 19
typedef struct customer {
  char name[MAXLEN + 1];
  int points;
} customer;

typedef struct treenode {
  customer *cPtr;
  int size;
  struct treenode *left;
  struct treenode *right;
} treenode;

treenode *insert(treenode *root, customer *customer);
customer *create_customer(char *name, int points);
treenode *create_node(customer *customer);
treenode *search(treenode *root, char *name);
treenode *searchdepth(treenode *root, char *name, int *depth);
treenode *add(treenode *root, char *name, int points);
treenode *sub(treenode *root, char *name, int points);
int count_smaller(treenode *root, char *name);
treenode *del(treenode *root, char *name);
treenode *inorderPre(treenode *root);
treenode *parent_finder(treenode *root, char *name);
void freenode(treenode *root);

int main() {
  int cmds;
  treenode *root = NULL;
  scanf("%d\n", &cmds);

  for (int i = 0; i < cmds; i++) {
    char command[MAXLEN];
    char name[MAXLEN];
    int points;
    scanf("%s %s", command, name);

    if (strcmp(command, "add") == 0) {
      scanf("%d", &points);
      treenode *existing_customer = search(root, name);

      if (existing_customer == NULL) {
        customer *new_customer = create_customer(name, points);
        root = insert(root, new_customer);
        printf("%s %d\n", name, new_customer->points);
      } else {
        treenode *new_points = add(root, name, points);
        printf("%s %d\n", name, new_points->cPtr->points);
      }

    } else if (strcmp(command, "sub") == 0) {
      scanf("%d", &points);
      treenode *existing_customer = search(root, name);

      if (existing_customer != NULL) {
        treenode *new_points = sub(root, name, points);
        printf("%s %d\n", name, new_points->cPtr->points);
      }

    } else if (strcmp(command, "del") == 0) {
      treenode *existing_customer = search(root, name);
      if (existing_customer != NULL) {
        del(root, name);
        printf("%s deleted\n", name);
      }

    } else if (strcmp(command, "search") == 0) {
      int depth = 0; // Initialize depth to 0
      treenode *temp = searchdepth(root, name, &depth);

      if (temp == NULL) {
        printf("%s not found\n", name);
      } else {
        printf("%s %d\n", name, depth);
      }
    } else if (strcmp(command, "count_smaller") == 0) {
      treenode *existing_customer = search(root, name);
      if (existing_customer != NULL) {
        int count = count_smaller(root, name);
        printf("%d\n", count);
      } else {
        printf("%s not found\n", name);
      }
    }
  }

  return 0;
}

treenode *insert(treenode *root, customer *customer) {
  if (root == NULL) {
    return create_node(customer);
  }

  int diff = strcmp(customer->name, root->cPtr->name);
  if (diff < 0) {
    root->left = insert(root->left, customer);
  } else if (diff > 0) {
    root->right = insert(root->right, customer);
  }

  root->size++;
  return root;
}

customer *create_customer(char *name, int points) {
  customer *new_customer = malloc(sizeof(customer));
  strcpy(new_customer->name, name);
  new_customer->points = points;
  return new_customer;
}
treenode *create_node(customer *customer) {
  treenode *temp = malloc(sizeof(treenode));
  temp->cPtr = customer;
  temp->size = 0;
  temp->left = NULL;
  temp->right = NULL;
  return temp;
}

treenode *search(treenode *root, char *name) {
  if (root == NULL) {
    return NULL;
  }
  int diff = strcmp(name, root->cPtr->name);
  if (diff < 0) {
    return search(root->left, name);

  } else if (diff > 0) {
    return search(root->right, name);
  } else if (diff == 0) {
    return root;
  }
}

treenode *searchdepth(treenode *root, char *name, int *depth) {
  if (root == NULL) {
    return NULL;
  }
  int diff = strcmp(name, root->cPtr->name);
  if (diff == 0) {
    return root;
  } else if (diff < 0) {
    treenode *found = searchdepth(root->left, name, depth);
    if (found != NULL)
      (*depth)++;
    return found;
  } else {
    treenode *found = searchdepth(root->right, name, depth);
    if (found != NULL)
      (*depth)++;
    return found;
  }
}

treenode *parent_finder(treenode *root, char *name) {
  if (root == NULL || (root->left == NULL && root->right == NULL)) {
    return NULL;
  }

  if (root->left != NULL && strcmp(root->left->cPtr->name, name) == 0) {
    return root;
  }

  // Check if the right child's name matches the given name
  if (root->right != NULL && strcmp(root->right->cPtr->name, name) == 0) {
    return root;
  }

  // Recursively search for the parent in the left subtree
  if (strcmp(name, root->cPtr->name) < 0) {
    return parent_finder(root->left, name);
  }

  // Recursively search for the parent in the right subtree
  return parent_finder(root->right, name);
}

treenode *add(treenode *root, char *name, int points) {
  treenode *temp = search(root, name);
  temp->cPtr->points += points;
  return temp;
}

treenode *sub(treenode *root, char *name, int points) {
  treenode *temp = search(root, name);
  temp->cPtr->points -= points;
  if (temp->cPtr->points < 0) {
    temp->cPtr->points = 0;
  }
  return temp;
}

int count_smaller(treenode *root, char *name) {
  treenode *temp = search(root, name);
  treenode *parent = parent_finder(root, name);

  if (temp == NULL) {
    return 1000;
  }

  else if (temp->left == NULL && parent == NULL) {
    return 5000;
  }

  else if (parent == NULL && temp->left != NULL) {
    return temp->left->size + 1;
  }

  else if (temp->left == NULL && temp == parent->right) {
    return parent->left->size + 2;
  }

  else if (parent->right == temp && temp->left != NULL) {
    return parent->left->size + temp->left->size + 3;
  }

  else if (parent->left == temp) {
    return temp->left->size + 1;
  }

  else if (temp == parent->left && temp->left != NULL) {
    return temp->left->size + 1;
  }
  return 0;
}

treenode *del(treenode *root, char *name) {
  if (root == NULL) {
    return NULL;
  }

  int diff = strcmp(name, root->cPtr->name);
  treenode *current = root;
  if (diff < 0) {

    current->left = del(current->left, name);
  } else if (diff > 0) {
    current->right = del(current->right, name);
  } else {
    // no children
    if (current->left == NULL && current->right == NULL) {
      free(current->cPtr);
      free(current);
      return NULL;
    }

    // one children
    else if (current->left == NULL) {
      treenode *temp = current->right;
      freenode(current);
      return temp;
    } 
    else if (current->right == NULL) {
      treenode *temp = current->left;
      freenode(current);
      return temp;
    }

    // two children
    else {
      treenode *temp = inorderPre(root->left);
      current->cPtr = temp->cPtr;
      current->left = del(current->left, temp->cPtr->name);
    }
  }
  return root;
}

treenode *inorderPre(treenode *root) {
  treenode *temp = root;
  while (temp->right != NULL)
    temp = temp->right;
  return temp;
}

void freenode(treenode *root) {
  free(root->cPtr);
  free(root);
}