#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>

#define ZOOM_SPEED 0.025f
#define MAX_NODES 5481

typedef struct node {
    Vector2 position;
    Color color;
    struct node *next;
} node_t;

void newNode(node_t **head, node_t node) {
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    newNode->position = node.position;
    newNode->color = node.color;
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

void check(node_t **head, node_t node) {
    bool nothing = true;
    node_t *current = *head;
    while (current != NULL) {
        if (current->position.x == node.position.x && current->position.y == node.position.y) {
            nothing = false;
            break;
        }
        current = current->next;
    }

    if (nothing) {
        newNode(head, node);
    }
}

void summonNodes(node_t **head, int layer, node_t *node1, node_t *node2, node_t *node3, Vector2 startPos1, Vector2 startPos2, Vector2 startPos3, float topValX, float bottomValX, float topValY, float bottomValY) {
    node1->position.x = startPos1.x + layer * topValX;
    node1->position.y = startPos1.y + layer * topValY;

    check(head, *node1);

    node2->position.x = startPos2.x + layer * topValX;
    node2->position.y = startPos2.y + layer * topValY;

    check(head, *node2);

    node3->position.x = startPos3.x + layer * topValX;
    node3->position.y = startPos3.y + layer * topValY;

    check(head, *node3);

    for (int i = 0; i < layer - 1; i++) {
        node1->position.x += bottomValX;
        node1->position.y += bottomValY;

        check(head, *node1);

        node2->position.x += bottomValX;
        node2->position.y += bottomValY;

        check(head, *node2);

        node3->position.x += bottomValX;
        node3->position.y += bottomValY;

        check(head, *node3);
    }
}

int main()
{
    const int winWidth = 800;
    const int winHeight = 448;

    InitWindow(winWidth, winHeight, "Grabby Aliens");

    node_t *head = NULL;

    Vector2 startPos1 = {176.0f, 192.0f};
    node_t node1 = {startPos1, RED, NULL};
    newNode(&head, node1);

    Vector2 startPos2 = {368.0f, 288.0f};
    node_t node2 = {startPos2, GREEN, NULL};
    newNode(&head, node2);

    Vector2 startPos3 = {592.0f, 96.0f};
    node_t node3 = {startPos3, BLUE, NULL};
    newNode(&head, node3);

    node_t *current = NULL;

    int layer = 1;

    double timer = 0;
    int count = 0;
    bool nothing = true;

    Camera2D camera = {0};
    camera.zoom = 0.25f;

    Vector2 mouseDelta = {0.0f, 0.0f};
    float mouseWheel = 0.0f;

    while (WindowShouldClose() == false) {
        mouseWheel = GetMouseWheelMove();
        if (mouseWheel != 0) {
            camera.target = GetScreenToWorld2D(GetMousePosition(), camera);
            camera.offset = GetMousePosition();
            camera.zoom = Clamp(camera.zoom + mouseWheel * ZOOM_SPEED, ZOOM_SPEED, 10.0f);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
            mouseDelta = GetMouseDelta();
            camera.target.x += -1 * mouseDelta.x / camera.zoom;
            camera.target.y += -1 * mouseDelta.y / camera.zoom;
        }

        timer -= GetFrameTime();

        count = 0;
        current = head;
        while (current != NULL) {
            count++;
            current = current->next;
        }

        if (timer < 0 && MAX_NODES > count) {
            // bottom right
            summonNodes(&head, layer, &node1, &node2, &node3, startPos1, startPos2, startPos3, 16, -16, 0, 16);
            // bottom left
            summonNodes(&head, layer, &node1, &node2, &node3, startPos1, startPos2, startPos3, 0, -16, 16, -16);
            // top left
            summonNodes(&head, layer, &node1, &node2, &node3, startPos1, startPos2, startPos3, -16, 16, 0, -16);
            // top right
            summonNodes(&head, layer, &node1, &node2, &node3, startPos1, startPos2, startPos3, 0, 16, -16, 16);

            layer += 1;
            timer = 0.3;
        }

        BeginDrawing();

        ClearBackground(WHITE);

        BeginMode2D(camera);

        current = head;
        while (current != NULL) {
            DrawRectangle(current->position.x, current->position.y, 16, 16, current->color);
            current = current->next;
        }

        EndMode2D();

        DrawFPS(5, 5);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
