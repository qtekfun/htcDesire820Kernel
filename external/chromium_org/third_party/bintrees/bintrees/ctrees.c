/*
 * ctrees.c
 *
 *  Author: mozman
 *  Copyright (c) 2010-2013 by Manfred Moitzi
 *  License: MIT-License
 */

#include "ctrees.h"
#include "stack.h"
#include <Python.h>

#define LEFT 0
#define RIGHT 1
#define KEY(node) (node->key)
#define VALUE(node) (node->value)
#define LEFT_NODE(node) (node->link[LEFT])
#define RIGHT_NODE(node) (node->link[RIGHT])
#define LINK(node, dir) (node->link[dir])
#define XDATA(node) (node->xdata)
#define RED(node) (node->xdata)
#define BALANCE(node) (node->xdata)

static node_t *
ct_new_node(PyObject *key, PyObject *value, int xdata)
{
	node_t *new_node = PyMem_Malloc(sizeof(node_t));
	if (new_node != NULL) {
		KEY(new_node) = key;
		Py_INCREF(key);
		VALUE(new_node) = value;
		Py_INCREF(value);
		LEFT_NODE(new_node) = NULL;
		RIGHT_NODE(new_node) = NULL;
		XDATA(new_node) = xdata;
	}
	return new_node;
}

static void
ct_delete_node(node_t *node)
{
	if (node != NULL) {
		Py_XDECREF(KEY(node));
		Py_XDECREF(VALUE(node));
		LEFT_NODE(node) = NULL;
		RIGHT_NODE(node) = NULL;
		PyMem_Free(node);
	}
}

extern void
ct_delete_tree(node_t *root)
{
	if (root == NULL)
		return;
	if (LEFT_NODE(root) != NULL) {
		ct_delete_tree(LEFT_NODE(root));
	}
	if (RIGHT_NODE(root) != NULL) {
		ct_delete_tree(RIGHT_NODE(root));
	}
	ct_delete_node(root);
}

static void
ct_swap_data(node_t *node1, node_t *node2)
{
	PyObject *tmp;
	tmp = KEY(node1);
	KEY(node1) = KEY(node2);
	KEY(node2) = tmp;
	tmp = VALUE(node1);
	VALUE(node1) = VALUE(node2);
	VALUE(node2) = tmp;
}

int
ct_compare(PyObject *key1, PyObject *key2)
{
	int res;

	res = PyObject_RichCompareBool(key1, key2, Py_LT);
	if (res > 0)
		return -1;
	else if (res < 0) {
		PyErr_SetString(PyExc_TypeError, "invalid type for key");
		return 0;
		}
	return PyObject_RichCompareBool(key1, key2, Py_GT);
}

extern node_t *
ct_find_node(node_t *root, PyObject *key)
{
	int res;
	while (root != NULL) {
		res = ct_compare(key, KEY(root));
		if (res == 0) 
			return root;
		else {
			root = LINK(root, (res > 0));
		}
	}
	return NULL; 
}

extern PyObject *
ct_get_item(node_t *root, PyObject *key)
{
	node_t *node;
	PyObject *tuple;

	node = ct_find_node(root, key);
	if (node != NULL) {
		tuple = PyTuple_New(2);
		PyTuple_SET_ITEM(tuple, 0, KEY(node));
		PyTuple_SET_ITEM(tuple, 1, VALUE(node));
		return tuple;
	}
	Py_RETURN_NONE;
}

extern node_t *
ct_max_node(node_t *root)
{
	if (root == NULL)
		return NULL;
	while (RIGHT_NODE(root) != NULL)
		root = RIGHT_NODE(root);
	return root;
}

extern node_t *
ct_min_node(node_t *root)
{
	if (root == NULL)
		return NULL;
	while (LEFT_NODE(root) != NULL)
		root = LEFT_NODE(root);
	return root;
}

extern int
ct_bintree_remove(node_t **rootaddr, PyObject *key)
{
	node_t *node, *parent, *replacement;
	int direction, cmp_res, down_dir;

	node = *rootaddr;

	if (node == NULL)
		return 0; 
	parent = NULL;
	direction = 0;

	while (1) {
		cmp_res = ct_compare(key, KEY(node));
		if (cmp_res == 0) 
		{
			if ((LEFT_NODE(node) != NULL) && (RIGHT_NODE(node) != NULL)) {
				
				parent = node;
				direction = RIGHT;
				replacement = RIGHT_NODE(node);
				while (LEFT_NODE(replacement) != NULL) {
					parent = replacement;
					direction = LEFT;
					replacement = LEFT_NODE(replacement);
				}
				LINK(parent, direction) = RIGHT_NODE(replacement);
				
				ct_swap_data(node, replacement);
				node = replacement; 
			}
			else {
				down_dir = (LEFT_NODE(node) == NULL) ? RIGHT : LEFT;
				if (parent == NULL) 
				{
					*rootaddr = LINK(node, down_dir);
				}
				else {
					LINK(parent, direction) = LINK(node, down_dir);
				}
			}
			ct_delete_node(node);
			return 1; 
		}
		else {
			direction = (cmp_res < 0) ? LEFT : RIGHT;
			parent = node;
			node = LINK(node, direction);
			if (node == NULL)
				return 0; 
		}
	}
}

extern int
ct_bintree_insert(node_t **rootaddr, PyObject *key, PyObject *value)
{
	node_t *parent, *node;
	int direction, cval;
	node = *rootaddr;
	if (node == NULL) {
		node = ct_new_node(key, value, 0); 
		if (node == NULL)
			return -1; 
		*rootaddr = node;
	}
	else {
		direction = LEFT;
		parent = NULL;
		while (1) {
			if (node == NULL) {
				node = ct_new_node(key, value, 0);
				if (node == NULL)
					return -1; 
				LINK(parent, direction) = node;
				return 1;
			}
			cval = ct_compare(key, KEY(node));
			if (cval == 0) {
				
				Py_XDECREF(VALUE(node)); 
				VALUE(node) = value; 
				Py_INCREF(value); 
				return 0;
			}
			else {
				parent = node;
				direction = (cval < 0) ? LEFT : RIGHT;
				node = LINK(node, direction);
			}
		}
	}
	return 1;
}

static int
is_red (node_t *node)
{
	return (node != NULL) && (RED(node) == 1);
}

#define rb_new_node(key, value) ct_new_node(key, value, 1)

static node_t *
rb_single(node_t *root, int dir)
{
	node_t *save = root->link[!dir];

	root->link[!dir] = save->link[dir];
	save->link[dir] = root;

	RED(root) = 1;
	RED(save) = 0;
	return save;
}

static node_t *
rb_double(node_t *root, int dir)
{
	root->link[!dir] = rb_single(root->link[!dir], !dir);
	return rb_single(root, dir);
}

#define rb_new_node(key, value) ct_new_node(key, value, 1)

extern int
rb_insert(node_t **rootaddr, PyObject *key, PyObject *value)
{
	node_t *root = *rootaddr;

	if (root == NULL) {
		root = rb_new_node(key, value);
		if (root == NULL)
			return -1; 
	}
	else {
		node_t head; 
		node_t *g, *t; 
		node_t *p, *q; 
		int dir = 0;
		int last = 0;
		int new_node = 0;

		
		t = &head;
		g = NULL;
		p = NULL;
		RIGHT_NODE(t) = root;
		LEFT_NODE(t) = NULL;
		q = RIGHT_NODE(t);

		
		for (;;) {
			int cmp_res;
			if (q == NULL) {
				
				q = rb_new_node(key, value);
				p->link[dir] = q;
				new_node = 1;
				if (q == NULL)
					return -1; 
			}
			else if (is_red(q->link[0]) && is_red(q->link[1])) {
				
				RED(q) = 1;
				RED(q->link[0]) = 0;
				RED(q->link[1]) = 0;
			}

			if (is_red(q) && is_red(p)) {
				
				int dir2 = (t->link[1] == g);

				if (q == p->link[last])
					t->link[dir2] = rb_single(g, !last);
				else
					t->link[dir2] = rb_double(g, !last);
			}

			
			if (new_node)
				break;

			cmp_res = ct_compare(KEY(q), key);
			if (cmp_res == 0) {       
				Py_XDECREF(VALUE(q)); 
				VALUE(q) = value;     
				Py_INCREF(value);     
				return 0;
			}
			last = dir;
			dir = (cmp_res < 0);

			
			if (g != NULL)
				t = g;

			g = p;
			p = q;
			q = q->link[dir];
		}
		
		root = head.link[1];
	}

	
	RED(root) = 0;
	(*rootaddr) = root;
	return 1;
}

extern int
rb_remove(node_t **rootaddr, PyObject *key)
{
	node_t *root = *rootaddr;

	node_t head = { { NULL } }; 
	node_t *q, *p, *g; 
	node_t *f = NULL; 
	int dir = 1;

	if (root == NULL)
		return 0;

	
	q = &head;
	g = p = NULL;
	RIGHT_NODE(q) = root;

	while (q->link[dir] != NULL) {
		int last = dir;
		int cmp_res;

		
		g = p, p = q;
		q = q->link[dir];

		cmp_res =  ct_compare(KEY(q), key);

		dir = cmp_res < 0;

		if (cmp_res == 0)
			f = q;

		
		if (!is_red(q) && !is_red(q->link[dir])) {
			if (is_red(q->link[!dir]))
				p = p->link[last] = rb_single(q, dir);
			else if (!is_red(q->link[!dir])) {
				node_t *s = p->link[!last];

				if (s != NULL) {
					if (!is_red(s->link[!last]) &&
						!is_red(s->link[last])) {
						
						RED(p) = 0;
						RED(s) = 1;
						RED(q) = 1;
					}
					else {
						int dir2 = g->link[1] == p;

						if (is_red(s->link[last]))
							g->link[dir2] = rb_double(p, last);
						else if (is_red(s->link[!last]))
							g->link[dir2] = rb_single(p, last);

						
						RED(q) = RED(g->link[dir2]) = 1;
						RED(g->link[dir2]->link[0]) = 0;
						RED(g->link[dir2]->link[1]) = 0;
					}
				}
			}
		}
	}

	
	if (f != NULL) {
		ct_swap_data(f, q);
		p->link[p->link[1] == q] = q->link[q->link[0] == NULL];
		ct_delete_node(q);
	}

	
	root = head.link[1];

	
	if (root != NULL)
		RED(root) = 0;
	*rootaddr = root;
	return (f != NULL);
}

#define avl_new_node(key, value) ct_new_node(key, value, 0)
#define height(p) ((p) == NULL ? -1 : (p)->xdata)
#define avl_max(a, b) ((a) > (b) ? (a) : (b))

static node_t *
avl_single(node_t *root, int dir)
{
  node_t *save = root->link[!dir];
	int rlh, rrh, slh;

	
	root->link[!dir] = save->link[dir];
	save->link[dir] = root;

	
	rlh = height(root->link[0]);
	rrh = height(root->link[1]);
	slh = height(save->link[!dir]);

	BALANCE(root) = avl_max(rlh, rrh) + 1;
	BALANCE(save) = avl_max(slh, BALANCE(root)) + 1;

	return save;
}

static node_t *
avl_double(node_t *root, int dir)
{
	root->link[!dir] = avl_single(root->link[!dir], !dir);
	return avl_single(root, dir);
}

extern int
avl_insert(node_t **rootaddr, PyObject *key, PyObject *value)
{
	node_t *root = *rootaddr;

	if (root == NULL) {
		root = avl_new_node(key, value);
		if (root == NULL)
			return -1; 
	}
	else {
		node_t *it, *up[32];
		int upd[32], top = 0;
		int done = 0;
		int cmp_res;

		it = root;
		
		for (;;) {
			
			cmp_res = ct_compare(KEY(it), key);
			if (cmp_res == 0) {
				Py_XDECREF(VALUE(it)); 
				VALUE(it) = value; 
				Py_INCREF(value); 
				return 0;
			}
			
			upd[top] = (cmp_res < 0);
			up[top++] = it;

			if (it->link[upd[top - 1]] == NULL)
				break;
			it = it->link[upd[top - 1]];
		}

		
		it->link[upd[top - 1]] = avl_new_node(key, value);
		if (it->link[upd[top - 1]] == NULL)
			return -1; 

		
		while (--top >= 0 && !done) {
			
			int lh, rh, max;

			cmp_res = ct_compare(KEY(up[top]), key);

			lh = height(up[top]->link[upd[top]]);
			rh = height(up[top]->link[!upd[top]]);

			
			if (lh - rh == 0)
				done = 1;
			if (lh - rh >= 2) {
				node_t *a = up[top]->link[upd[top]]->link[upd[top]];
				node_t *b = up[top]->link[upd[top]]->link[!upd[top]];

				if (height( a ) >= height( b ))
					up[top] = avl_single(up[top], !upd[top]);
				else
					up[top] = avl_double(up[top], !upd[top]);

				
				if (top != 0)
					up[top - 1]->link[upd[top - 1]] = up[top];
				else
					root = up[0];
				done = 1;
			}
			
			lh = height(up[top]->link[upd[top]]);
			rh = height(up[top]->link[!upd[top]]);
			max = avl_max(lh, rh);
			BALANCE(up[top]) = max + 1;
		}
	}
	(*rootaddr) = root;
	return 1;
}

extern int
avl_remove(node_t **rootaddr, PyObject *key)
{
	node_t *root = *rootaddr;
	int cmp_res;

	if (root != NULL) {
		node_t *it, *up[32];
		int upd[32], top = 0;

		it = root;
		for (;;) {
			
			if (it == NULL)
				return 0;
			cmp_res = ct_compare(KEY(it), key);
			if (cmp_res == 0)
				break;

			
			upd[top] = (cmp_res < 0);
			up[top++] = it;
			it = it->link[upd[top - 1]];
		}

		
		if (it->link[0] == NULL ||
			it->link[1] == NULL) {
			
			int dir = it->link[0] == NULL;

			
			if (top != 0)
				up[top - 1]->link[upd[top - 1]] = it->link[dir];
			else
				root = it->link[dir];

			ct_delete_node(it);
		}
		else {
			
			node_t *heir = it->link[1];

			
			upd[top] = 1;
			up[top++] = it;

			while ( heir->link[0] != NULL ) {
				upd[top] = 0;
				up[top++] = heir;
				heir = heir->link[0];
			}
			
			ct_swap_data(it, heir);
			
			up[top - 1]->link[up[top - 1] == it] = heir->link[1];
			ct_delete_node(heir);
		}

		
		while (--top >= 0) {
			int lh = height(up[top]->link[upd[top]]);
			int rh = height(up[top]->link[!upd[top]]);
			int max = avl_max(lh, rh);

			
			BALANCE(up[top]) = max + 1;

			
			if (lh - rh == -1)
				break;
			if (lh - rh <= -2) {
				node_t *a = up[top]->link[!upd[top]]->link[upd[top]];
				node_t *b = up[top]->link[!upd[top]]->link[!upd[top]];

				if (height(a) <= height(b))
					up[top] = avl_single(up[top], upd[top]);
				else
					up[top] = avl_double(up[top], upd[top]);

				
				if (top != 0)
					up[top - 1]->link[upd[top - 1]] = up[top];
				else
					root = up[0];
			}
		}
	}
	(*rootaddr) = root;
	return 1;
}

extern node_t *
ct_succ_node(node_t *root, PyObject *key)
{
	node_t *succ = NULL;
	node_t *node = root;
	int cval;

	while (node != NULL) {
		cval = ct_compare(key, KEY(node));
		if (cval == 0)
			break;
		else if (cval < 0) {
			if ((succ == NULL) ||
				(ct_compare(KEY(node), KEY(succ)) < 0))
				succ = node;
			node = LEFT_NODE(node);
		} else
			node = RIGHT_NODE(node);
	}
	if (node == NULL)
		return NULL;
	
	if (RIGHT_NODE(node) != NULL) {
		
		node = RIGHT_NODE(node);
		while (LEFT_NODE(node) != NULL)
			node = LEFT_NODE(node);
		if (succ == NULL)
			succ = node;
		else if (ct_compare(KEY(node), KEY(succ)) < 0)
			succ = node;
	}
	return succ;
}

extern node_t *
ct_prev_node(node_t *root, PyObject *key)
{
	node_t *prev = NULL;
	node_t *node = root;
	int cval;

	while (node != NULL) {
		cval = ct_compare(key, KEY(node));
		if (cval == 0)
			break;
		else if (cval < 0)
			node = LEFT_NODE(node);
		else {
			if ((prev == NULL) || (ct_compare(KEY(node), KEY(prev)) > 0))
				prev = node;
			node = RIGHT_NODE(node);
		}
	}
	if (node == NULL) 
		return NULL;
	
	if (LEFT_NODE(node) != NULL) {
		
		node = LEFT_NODE(node);
		while (RIGHT_NODE(node) != NULL)
			node = RIGHT_NODE(node);
		if (prev == NULL)
			prev = node;
		else if (ct_compare(KEY(node), KEY(prev)) > 0)
			prev = node;
	}
	return prev;
}

extern node_t *
ct_floor_node(node_t *root, PyObject *key)
{
	node_t *prev = NULL;
	node_t *node = root;
	int cval;

	while (node != NULL) {
		cval = ct_compare(key, KEY(node));
		if (cval == 0)
			return node;
		else if (cval < 0)
			node = LEFT_NODE(node);
		else {
			if ((prev == NULL) || (ct_compare(KEY(node), KEY(prev)) > 0))
				prev = node;
			node = RIGHT_NODE(node);
		}
	}
	return prev;
}

extern node_t *
ct_ceiling_node(node_t *root, PyObject *key)
{
	node_t *succ = NULL;
	node_t *node = root;
	int cval;

	while (node != NULL) {
		cval = ct_compare(key, KEY(node));
		if (cval == 0)
			return node;
		else if (cval < 0) {
			if ((succ == NULL) ||
				(ct_compare(KEY(node), KEY(succ)) < 0))
				succ = node;
			node = LEFT_NODE(node);
		} else
			node = RIGHT_NODE(node);
	}
	return succ;
}

extern int
ct_index_of(node_t *root, PyObject *key)
{
	node_t *node = root;
	int index = 0;
	int go_down = 1;
	node_stack_t *stack;
	stack = stack_init(32);

	for (;;) {
		if ((LEFT_NODE(node) != NULL) && go_down) {
			stack_push(stack, node);
			node = LEFT_NODE(node);
		}
		else {
			if (ct_compare(KEY(node), key) == 0) {
				stack_delete(stack);
				return index;
			}
			index++;
			if (RIGHT_NODE(node) != NULL) {
				node = RIGHT_NODE(node);
				go_down = 1;
			}
			else {
				if (stack_is_empty(stack)) {
					stack_delete(stack);
					return -1;
				}
				node = stack_pop(stack);
				go_down = 0;
			}
		}
	}
}

extern node_t *
ct_node_at(node_t *root, int index)
{
	node_t *node = root;
	int counter = 0;
	int go_down = 1;
	node_stack_t *stack;

	if (index < 0) return NULL;

	stack = stack_init(32);

	for(;;) {
		if ((LEFT_NODE(node) != NULL) && go_down) {
			stack_push(stack, node);
			node = LEFT_NODE(node);
		}
		else {
			if (counter == index) {
				
				stack_delete(stack);
				return node;
			}
			counter++;
			if (RIGHT_NODE(node) != NULL) {
				node = RIGHT_NODE(node);
				go_down = 1;
			}
			else {
				if (stack_is_empty(stack)) { 
					stack_delete(stack);
					return NULL;
                }
				node = stack_pop(stack);
				go_down = 0;
			}
		}
    }
}
