#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    Vector2 position;
    struct node *next;
} node_t;

void newNode(node_t **head, node_t node) {
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    newNode->position = node.position;
    newNode->next = NULL;

    if (*head == NULL) {
        *head = newNode;
    } else {
        node_t *current = *head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void popHead(node_t **head) {
    if (*head == NULL) {
        fprintf(stderr, "ERROR: underflow, can't pop from empty list\n");
        exit(1);
    }
    node_t *nextNode = (*head)->next;
    free(*head);
    *head = nextNode;
}

void popLast(node_t **head) {
    if (*head == NULL) {
        fprintf(stderr, "ERROR: underflow, can't pop from empty list\n");
        exit(1);
    } else if ((*head)->next == NULL) {
        node_t *nextNode = (*head)->next;
        free(*head);
        *head = nextNode;
    } else {
        node_t *current = *head;
        while (current->next->next != NULL) {
            current = current->next;
        }
        free(current->next);
        current->next = NULL;
    }
}

void popIndex(node_t **head, int index) {
    if (index == 0) {
        popHead(head);
    } else {
        node_t *current = *head;
        for (int i = 0; i < index - 1; i++) {
            if (current->next == NULL) {
                fprintf(stderr, "ERROR: index out of range\n");
                exit(1);
            }
            current = current->next;
        }
        if (current->next == NULL) {
            fprintf(stderr, "ERROR: index out of range\n");
            exit(1);
        }
        node_t *tempNode = current->next;
        current->next = tempNode->next;
        free(tempNode);
    }
}

int main() {
    const int winWidth = 800;
    const int winHeight = 448;

    InitWindow(winWidth, winHeight, "Grabby Aliens");

    node_t *head = NULL;

    Vector2 startPos = {400.0f, 224.0f};
    node_t node = {startPos, NULL};

    newNode(&head, node);

    node_t *current1 = NULL;
    node_t *current2 = NULL;

    int layer = 1;

    double timer = 0;

    SetTargetFPS(60);

    while (WindowShouldClose() == false) {
        timer -= GetFrameTime();
        if (timer < 0) {
            node.position.x = startPos.x + layer * 16.0f;
            node.position.y = startPos.y;
            newNode(&head, node);
            for (int i = 0; i < layer - 1; i++) {
                node.position.x -= 16.0f;
                node.position.y += 16.0f;
                newNode(&head, node);
            }
            node.position.x = startPos.x;
            node.position.y = startPos.y + layer * 16.0f;
            newNode(&head, node);
            for (int i = 0; i < layer - 1; i++) {
                node.position.x -= 16.0f;
                node.position.y -= 16.0f;
                newNode(&head, node);
            }
            node.position.x = startPos.x + layer * -16.0f;
            node.position.y = startPos.y;
            newNode(&head, node);
            for (int i = 0; i < layer - 1; i++) {
                node.position.x += 16.0f;
                node.position.y -= 16.0f;
                newNode(&head, node);
            }
            node.position.x = startPos.x;
            node.position.y = startPos.y + layer * -16.0f;
            newNode(&head, node);
            for (int i = 0; i < layer - 1; i++) {
                node.position.x += 16.0f;
                node.position.y += 16.0f;
                newNode(&head, node);
            }
            layer += 1;
            timer = 0.5;
        }

        BeginDrawing();

        ClearBackground(WHITE);

        current1 = head;
        while (current1 != NULL) {
            DrawRectangle(current1->position.x, current1->position.y, 16, 16, RED);
            current1 = current1->next;
        }

        DrawText(TextFormat("%d", GetFPS()), 5, 5, 25, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
