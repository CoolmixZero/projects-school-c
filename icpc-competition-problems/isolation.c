#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct big_tree {
    struct big_tree* left;
    struct big_tree* right;
    struct big_tree* parent;
    int data;
} big_tree;

big_tree* createTreeRoot(int data) {
    big_tree* root = (big_tree*) malloc(sizeof(big_tree));

    root->parent = root->left = root->right = NULL;
    root->data = data;

    return root;
}

big_tree* addbig_tree(big_tree* root, int data) {
    big_tree* current_big_tree = (big_tree*) malloc(sizeof(big_tree));

    current_big_tree->data = data;
    current_big_tree->left = NULL;
    current_big_tree->right = NULL;

    big_tree* parent = NULL;
    big_tree* search_big_tree = root;

    while (search_big_tree != NULL) {
        parent = search_big_tree;
        if (data < search_big_tree->data) {
            search_big_tree = search_big_tree->left;
        } else {
            search_big_tree = search_big_tree->right;
        }
    }

    current_big_tree->parent = parent;

    if (data < parent->data) {
        parent->left = current_big_tree;
    } else {
        parent->right = current_big_tree;
    }

    return root;
}

bool comparePeer(big_tree* peer1, big_tree* peer2) {
    if (peer1 == NULL && peer2 != NULL) {
        return false;
    }
    if (peer1 != NULL && peer2 == NULL) {
        return false;
    }

    return true;
}

bool compareTrees(big_tree* root1, big_tree* root2) {
    if (root1->left == NULL && root1->right == NULL && root2->left == NULL && root2->right == NULL) {
        return true;
    }

    if (!comparePeer(root1->left, root2->left)) {
        return false;
    }
    if (!comparePeer(root1->right, root2->right)) {
        return false;
    }

    bool result = true;
    if (root1->left != NULL) {
        result = result && compareTrees(root1->left, root2->left);
    }
    if (root1->right != NULL) {
        result = result && compareTrees(root1->right, root2->right);
    }

    return result;
}

void treeDelete(big_tree* root) {
    if (root->left != NULL) {
        treeDelete(root->left);
    }
    if (root->right != NULL) {
        treeDelete(root->right);
    }

    free(root);
}

int main() {
    int tree_count = 0;
    int big_tree_count = 0;

    scanf("%d%d", &tree_count, &big_tree_count);

    big_tree* trees[tree_count];

    for (int i = 0; i < tree_count; ++i) {
        int root_data = 0;
        scanf("%d", &root_data);

        big_tree* tree = createTreeRoot(root_data);
        trees[i] = tree;

        for (int j = 0; j < big_tree_count - 1; ++j) {
            int big_tree_data = 0;
            scanf("%d", &big_tree_data);

            addbig_tree(tree, big_tree_data);
        }
    }

    int tree_types_count = 0;

    for (int i = 0; i < tree_count; ++i) {
        bool has_same_tree = false;

        for (int j = 0; j < i; ++j) {
            if (compareTrees(trees[i], trees[j])) {
                has_same_tree = true;
                break;
            }
        }

        if (!has_same_tree) {
            ++tree_types_count;
        }
    }

    for (int i = 0; i < tree_count; ++i) {
        treeDelete(trees[i]);
    }
    char ch_result[10000] = {0};
    sprintf(ch_result, "%d", tree_types_count);
    printf("%s\n", ch_result);

    return 0;
}
