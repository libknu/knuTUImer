#ifndef __LIST_H__
#define __LIST_H__

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}t_list;

t_list	*lstnew(void *content);
void	lstadd_front(t_list **lst, t_list *new);
int		lstsize(t_list *lst);
t_list	*lstlast(t_list *lst);
void	lstadd_back(t_list **lst, t_list *new);
void	lstdelone(t_list *lst, void (*del)(void *));
void	lstclear(t_list **lst, void (*del)(void *));
void	lstiter(t_list *lst, void (*f)(void *));
t_list	*lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));


#endif
