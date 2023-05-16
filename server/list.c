#include "list.h"

void	lstadd_back(t_list **lst, t_list *new)
{
	t_list	*res;

	res = *lst;
	if (new == NULL)
		return ;
	if (*lst == NULL && new != NULL)
	{
		*lst = new;
		return ;
	}
	while ((*lst)->next)
		*lst = (*lst)->next;
	(*lst)->next = new;
	*lst = res;
}

void	lstadd_front(t_list **lst, t_list *new)
{
	if (new == NULL)
		return ;
	new->next = *lst;
	*lst = new;
}

void	lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*cur;

	while (*lst)
	{
		cur = (*lst)->next;
		lstdelone(*lst, del);
		*lst = cur;
	}
}

void	lstdelone(t_list *lst, void (*del)(void *))
{
	if (del)
		del(lst->content);
	free(lst);
}

void	lstiter(t_list *lst, void (*f)(void *))
{
	while (lst)
	{
		f(lst->content);
		lst = lst->next;
	}
}

t_list	*lstlast(t_list *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

t_list	*lstnew(void *content)
{
	t_list	*new_node;

	new_node = (t_list *)malloc(sizeof(t_list));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}

int	lstsize(t_list *lst)
{
	int	size;

	size = 0;
	while (lst)
	{
		lst = lst->next;
		size++;
	}
	return (size);
}

t_list	*lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*new_node;

	new_lst = NULL;
	while (lst)
	{
		new_node = lstnew(f(lst->content));
		if (!new_node)
		{
			lstclear(&new_lst, del);
			return (NULL);
		}
		lstadd_back(&new_lst, new_node);
		lst = lst->next;
	}
	return (new_lst);
}
