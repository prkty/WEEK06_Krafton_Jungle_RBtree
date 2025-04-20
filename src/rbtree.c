#include "rbtree.h"

#include <stdlib.h>

// 새 트리 생성 함수
rbtree *new_rbtree(void) {
  // tree 구조체 동적 할당
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  
  // NIL 노드 생성 및 초기화
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  if (nil == NULL)
    {
      free(p);
      return NULL;
    }
  nil -> color = RBTREE_BLACK;  // NIL 노드는 항상 Black
  nil->key = 0;
  nil->parent = NULL;
  nil->left = NULL;
  nil->right = NULL;
  // NIL 노드 생성 및 초기화

  // tree의 nil과 root를 nil 노드로 설정 (tree가 빈 경우 root는 nil노드여야 한다.)
  // p -> nil = p -> root = nil;

  return p;
}

// 왼쪽 회전
void left_rotation(rbtree *t, node_t *x) {
  // x가 위에 있는 부모이고, y는 x의 오른쪽 아래의 자식이다.
  node_t *y = x -> right;  // x의 오른쪽 자식은 노드 y 이다.
  x -> right = y -> left;  // y의 왼쪽 자식을 x의 오른쪽 자식으로 회전한다.

  if ( y -> left !=  t -> nil) {   // y의 왼쪽 자식이 비어 있지 않은 경우
    y -> left -> parent = x;       // y의 왼쪽 자식의 부모는 x가 된다.
  }

  y -> parent = x -> parent;       // x의 부모는 y의 부모가 된다.

  // 여기까지 y를 x자리로 올리기 위해 y의 자식을 x의 자식으로 처리하고 y를 x의 부모로 대체하는 과정이다. (y에 자식이 있을때)

  if (x -> parent == t -> nil) {  // x의 부모가 t는 nil(없을때)일때, 즉 x가 루트일때
    t -> root = y;                // y가 트리의 루트가 된다.
  }

  else if (x == x -> parent -> left) {   // x가 부모의 왼쪽 자식일때
    x -> parent -> left = y;             // x의 부모의 왼쪽 자식을 y로 대체
  }
  
  else {                         // 그외의 경우(x는 부모의 오른쪽 자식이었다
    x -> parent -> right = y;    // x의 부모의 오른쪽 자식을 y로 대체
  }

  y -> left = x;     // x를 y의 왼쪽 자식으로 둔다
  x -> parent = y;   // y룰 x의 부모로 둔다

  // 여기까지 x가 루트였으면 y를 루트로 두고, x가 부모가 있었으면 그 부모를 y의 부모로 바꾸는 과정이다.
  // 이후 마지막 두 문장은 x와 y의 부모, 자식 관계를 재정립한다.
}

// 오른쪽 회전
void right_rotation(rbtree *t, node_t *x) {
  // x가 위에 있는 부모이고, y는 x의 왼쪽 아래의 자식이다.
  node_t *y = x -> left;  // x의 왼쪽 자식은 노드 y 이다.
  x -> left = y -> right;  // y의 오른쪽 자식을 x의 왼쪽 자식으로 회전한다.

  if ( y -> right !=  t -> nil) {   // y의 오른쪽 자식이 비어 있지 않은 경우
    y -> right -> parent = x;       // y의 오른쪽 자식의 부모는 x가 된다.
  }

  y -> parent = x -> parent;       // x의 부모는 y의 부모가 된다.

  // 여기까지 y를 x자리로 올리기 위해 y의 자식을 x의 자식으로 처리하고 y를 x의 부모로 대체하는 과정이다. (y에 자식이 있을때)

  if (x -> parent == t -> nil) {  // x의 부모가 t는 nil(없을때)일때, 즉 x가 루트일때
    t -> root = y;                // y가 트리의 루트가 된다.
  }

  else if (x == x -> parent -> left) {   // x가 부모의 왼쪽 자식일때
    x -> parent -> left = y;             // x의 부모의 왼쪽 자식을 y로 대체
  }
  
  else {                         // 그외의 경우(x는 부모의 오른쪽 자식이었다
    x -> parent -> right = y;    // x의 부모의 오른쪽 자식을 y로 대체
  }

  y -> left = x;     // x를 y의 왼쪽 자식으로 둔다
  x -> parent = y;   // y룰 x의 부모로 둔다

  // 여기까지 x가 루트였으면 y를 루트로 두고, x가 부모가 있었으면 그 부모를 y의 부모로 바꾸는 과정이다.
  // 이후 마지막 두 문장은 x와 y의 부모, 자식 관계를 재정립한다.
}

// 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // 책에서는 새 노드가 z라고 나오는데, 
  // 해당 코드에선 z가 new_node로, new_node에 이미 키가 삽입되어 있다.

  // 노드생성
  node_t *new_node = (node_t *)calloc(1, sizeof(node_t));

  // 새로 추가할 노드 값 초기화
  new_node -> key = key;           // 새 노드안에 들어가는 값은 key
  // new_node -> color = RBTREE_RED;  // 무조건 새 노드 들어갈때 빨강 지정
  // new_node -> left = t -> nil;     // 새 노드의 왼쪽 자식 노드는 nil
  // new_node -> right = t -> nil;    // 새 노드의 오른쪽 자식 노드는 nil

  node_t *x = t -> root;  // x는 루트 노드에서 시작 (트리를 따라 내려감)
  // 현재 탐색 중인 노드 x(비교 대상)
  node_t *y = t -> nil;   // y는 x의 부모를 기억할 변수 (초기에는 nil)
  // y는 삽입될 위치의 부모 노드

  // 삽입 시작
  while (x != t -> nil) {   // x가 nil일 때 까지 내려간다(경계 NIL에 도달할 때까지 내려간다)
    y = x;   // 초기에는 y는 x이다. 
    // 밑으로 내려가면서 새 노드를 삽입할 적당한 위치를 찾는다.

    if (new_node -> key < x -> key) {  // 새노드의 키(값)가 x노드의 키(값)보다 작을때
      x = x -> left;         // x노드는 x의 오른쪽에 온다
    }
      else {                 // 반대의 경우
        x = x -> right;      // x노드는 x의 오른쪽에 온다
      }
    new_node -> parent = y;  // y는 새 노드의 부모님으로 지정
    // 여기까지 탐색대상 x를 내리며 값이 작으면 왼쪽 자식을 탐색하고 크면 오른쪽 자식을 탐색한다.

    if (y == t -> nil) {     // y가 nil이면(트리에 아무것도 없었다)
      t -> root = new_node;  // 새 노드는 루트이다
    }
    else if (new_node -> key < y -> key) {  // 새 노드의 키가 y(부모)의 키보다 작으면
      y -> left = new_node;  // 새 노드는 y의 왼쪽 자식에 온다
    }
    else {  // 그외의 케이스, 새 노드가 y보다 크면
      y -> right = new_node; // 새 노드는 y의 오른쪽 자식에 온다
    }
    // 새노드를 삽입하기 위해 부모 y를 기준으로 키값을 비교하여 자식값에 넣는다.

    // 새노드에 왼쪽, 오른쪽 자식에 NIL을 추가, 새 노드는 빨강으로 지정
    new_node -> left = t -> nil;    
    new_node -> right = t -> nil;
    new_node -> color = RBTREE_RED;

    // 수정 작업 수행(트리와 새 노드값을 넘김)
    insert_fix(t, new_node);
    }
  return t->root;
}

// 삽입에서 넘긴 값을 토대로 수정 수행
void insert_fix(rbtree *t, node_t *new_node) {
  node_t *uncle; // 삼촌을 정의

while (new_node -> parent -> color == RBTREE_RED) {   // 삽입한 부모 노드의 색깔이 Red 일때까지 실행

    // 부모가 왼쪽일때 과정
    if (new_node->parent == new_node->parent->parent->left) {  // 새 노드의 부모가 조부모의 왼쪽일때
      uncle = new_node->parent->parent->right;      // 삼촌은 새 노드의 조부모의 오른쪽 자식임

      // Case 1
      if (uncle -> color == RBTREE_RED) {           // 삼촌의 색이 빨간색인 케이스
        new_node -> parent -> color = RBTREE_BLACK; // 새 노드의 부모는 검은색으로 지정
        uncle -> color = RBTREE_BLACK;              // 삼촌 또한 검은색으로 지정
        new_node -> parent -> parent = RBTREE_RED;  // 조부모는 빨간색으로 지정
        new_node = new_node -> parent -> parent;    // 조부모까진 완벽하니, 조부모를 새 노드로 지정하고 재확인
      }
      else {
        if (new_node == new_node -> parent -> right) {  // 새 노드가 부모의 오른쪽에 있을때
          // Case 2
          new_node = new_node -> parent;               // 새 노드를 부모로 지정
          left_rotation(t, new_node);                  // 새 노드의 부모를 기준으로 왼쪽 로테이션 돌림
          // Case 3(위치는 바꿨으니 색을 바꿔줘야함)
          new_node -> parent -> color = RBTREE_BLACK;  // 새 노드의 부모를 검은색으로 지정
          new_node -> parent -> parent -> color = RBTREE_RED;  // 조부모는 빨간색으로 지정
          right_rotation(t, new_node -> parent -> parent);     // 조부모 기준으로 오른쪽 로테이션 돌림
        }
      }
    }

    // 위의 케이스와 다르게 부모가 오른쪽일때 과정
    else {
      uncle = new_node->parent->parent->left;      // 삼촌은 새 노드의 조부모의 왼쪽 자식임

      // Case 1
      if (uncle -> color == RBTREE_RED) {           // 삼촌의 색이 빨간색인 케이스
        new_node -> parent -> color = RBTREE_BLACK; // 새 노드의 부모는 검은색으로 지정
        uncle -> color = RBTREE_BLACK;              // 삼촌 또한 검은색으로 지정
        new_node -> parent -> parent = RBTREE_RED;  // 조부모는 빨간색으로 지정
        new_node = new_node -> parent -> parent;    // 조부모까진 완벽하니, 조부모를 새 노드로 지정하고 재확인
      }
      else {
        if (new_node == new_node -> parent -> left) {  // 새 노드가 부모의 오른쪽에 있을때
          // Case 2
          new_node = new_node -> parent;               // 새 노드를 부모로 지정
          right_rotation(t, new_node);                  // 새 노드의 부모를 기준으로 오른쪽 로테이션 돌림
          // Case 3(위치는 바꿨으니 색을 바꿔줘야함)
          new_node -> parent -> color = RBTREE_BLACK;  // 새 노드의 부모를 검은색으로 지정
          new_node -> parent -> parent -> color = RBTREE_RED;  // 조부모는 빨간색으로 지정
          left_rotation(t, new_node -> parent -> parent);     // 조부모 기준으로 왼쪽 로테이션 돌림
        }
      }
    }
  }
  t -> root -> color = RBTREE_BLACK;  // 루트는 항상 검은색이어야하는 규칙(루트의 부모는 없기 때문에 색을 검은색으로 지정해도됨)
}



void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
