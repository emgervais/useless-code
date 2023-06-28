#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

static int	ft_ilen(long int n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		count++;
		n *= -1;
	}
	while (n > 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char		*f;
	int			l;
	int			i;
	long int	nb;

	nb = n;
	l = ft_ilen(nb);
	i = 0;
	f = malloc(sizeof(char) * (l + 1));
	if (!(f))
		return (NULL);
	f[l--] = 0;
	if (nb < 0)
	{
		f[i++] = '-';
		nb *= -1;
	}
	while (l >= i)
	{
		f[l] = nb % 10 + 48;
		nb /= 10;
		l--;
	}
	return (f);
}

int valid(char *av)
{
    int i = 0;

    while(av[i])
    {
        if(av[i] <= '9' && av[i] >= '0')
            i++;
        else
            return(1);
    }
    return (0);
}

void put_string(char *a, int fd)
{
    int i = 0;
    while(a[i])
        write(fd, &a[i++], 1);
}

void write_begin(int fd, char *nbr)
{
    int i = 0;
    while(nbr[i])
        i++;
    write(fd, "#include <stdio.h>\n#include <unistd.h>\n\nint main()\n{\n\tint i = ", 62);
    put_string(nbr, fd);
}
void write_program(char *nbr)
{
    int i = 1;
    int fd = open("./pair.c", O_WRONLY | O_CREAT | O_TRUNC, 0777);
    write_begin(fd, nbr);
    write(fd, ";\n\n\tif(i == 0)\n\t{\n\t\tprintf(\"pair!\\n\");\n\t\treturn(0);\n\t}\n\t", 56);
    while(i <= 10000)
    {
        write(fd, "else if(i == ", 13);
        put_string(ft_itoa(i), fd);
        if(i % 2 == 1)
            write(fd, ")\n\t{\n\t\tprintf(\"impair!\\n\");\n\t\treturn(0);\n\t}\n\t", 45);
        else
            write(fd, ")\n\t{\n\t\tprintf(\"pair!\\n\");\n\t\treturn(0);\n\t}\n\t", 43);
        i++;
    }
    write(fd, "}", 1);
}

int main(int ac, char **av, char **envp)
{
    if(ac != 2 || valid(av[1]))
        return(1);
    long nbr = atoi(av[1]);
    if(nbr > 10000 || nbr < 0)
        return (printf("number needs to be between 0 and 10000\n"), 0);
    write_program(av[1]);
    char *args[] = {"gcc", "pair.c", "-o", "exec", NULL};
    char *args2[] = {"./exec", NULL};
    pid_t pid = fork();
    if(pid == 0)
        execve("/usr/bin/gcc", args, envp);
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if(WIFEXITED(status) && WEXITSTATUS(status) == 0)
        {
            pid_t exec_pid = fork();
            if(exec_pid == 0)
                execve("./exec", args2, envp);
            else
            {
                waitpid(exec_pid, &status, 0);
                char *args[] = {"/bin/rm", "pair.c", "a.out", "exec", NULL};
	            //execve("/bin/rm", args, NULL);
            }
        }
    }
}
