#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
	enum {
		ADD,
		MULTI,
		VAL
	}	type;
	int			val;
	struct node	*l;
	struct node	*r;
}	node;

node	*parse_expr(char **s);
void    destroy_tree(node *n);

void	unexpected(char **s)
{
	if (**s == '\0')
		printf("$unexpected end of input\n");
	else
		printf("$unexpected '%c'\n", **s);
}

node	*new_node(node n)
{
	node	*ret = calloc(1, sizeof(node));
	if (!ret)
		return NULL;
	*ret = n;
	return ret;
}

int	accept(char **s, char c)
{
	if (**s == c)
	{
		(*s)++;
		return 1;
	}
	return 0;
}

int	expect(char **s, char c)
{
	if (accept(s, c))
		return 1;
	unexpected(s);
	return 0;
}

node	*parse_factor(char **s)
{
	node	n;
	if (accept(s, '('))
	{
		node	*expr = parse_expr(s);
		if(!expr || !accept(s, ')'))
		{
			if (expr)
				destroy_tree(expr);
			unexpected(s);
			return NULL;
		}
		return (expr);
	}
	if (**s >= '0' && **s <= '9')
	{
		n.type = VAL;
		n.val = **s - '0';
		(*s)++;
		return (new_node(n));
	}
	return NULL;
}

node	*parse_term(char **s)
{
	node	*left = parse_factor(s);
	while (accept(s, '*'))
	{
		node	n;
		n.type = MULTI;
		n.l = left;
		n.r = parse_term(s);
		if (!n.r)
		{
			destroy_tree(left);
			return NULL;
		}
		node *new = new_node(n);
        if (!new)
        {
            destroy_tree(left);
            destroy_tree(n.r);
            return NULL;
        }
		left = new;
	}
	return left;
}

node	*parse_expr(char **s)
{
	node	*ret = parse_term(s);
	while (accept(s, '+'))
	{
		node	n;
		n.type = ADD;
		n.l = ret;
		n.r = parse_term(s);
		if (!n.r)
		{
			destroy_tree(ret);
			unexpected(s);
			return NULL;
		}
		ret = new_node(n);
		if (!ret)   // Need to add this check
        {
            destroy_tree(n.l);
            destroy_tree(n.r);
            return NULL;
        }
	}
	return ret;
}

void	destroy_tree(node *n)
{
	if (!n)
		return;
	destroy_tree(n->l);
	destroy_tree(n->r);
	free(n);
}

int	eval_tree(node *tree)
{
	if (!tree)
		return 0;
	switch (tree->type)
	{
		case ADD:
			return (eval_tree(tree->l) + eval_tree(tree->r));
		case MULTI:
			return (eval_tree(tree->l) * eval_tree(tree->r));
		case VAL:
			return (tree->val);
	}
	return 0;
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return 1;
	char	*arg = argv[1];
	node	*tree = parse_expr(&arg);
	if (*arg)
	{
		unexpected(&arg);
		return 1;
	}
	else if (!tree)
	{
		unexpected(&arg);
		return 1;
	}
	printf("%d\n", eval_tree(tree));
	destroy_tree(tree);
	return 0;
}
