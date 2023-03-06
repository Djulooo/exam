#include <unistd.h>
#include <stdarg.h>

static void ft_putnbr_base(long long d, int baselen, char *base, int *count)
{
    int div;
    int res;

    div = 1;
    while (d / div != 0)
        div *= baselen;
    while (div != 0)
    {
        res = d / div % baselen;
        write(1, base[res], 1);
        div /= baselen;
        *count += 1;
    }
}

static void dec(va_list arg, int *count)
{
    int d;

    d = va_arg(arg, int);
    if (d == -2147483648)
    {
        write(1, "-2147483648", 11);
        *count += 1;
        return ;
    }
    if (d < 0)
    {
        write(1, "-", 1);
        *count += 1;
        d *= -1;
    }
    ft_putnbr_base(d, 10, "0123456789", count);
}

static void hex(va_list arg, int *count)
{
    unsigned int d;

    d = va_arg(arg, unsigned int);
    ft_putnbr_base(d, 16, "0123456789abcdef", count);
}

static void str(va_list arg, int *count)
{
    int i;
    char *str;

    i = 0;
    str = va_arg(arg, char *);
    if (str == NULL)
    {
        write(1, "(null)", 6);
        *count += 6;
    }
    else
    {
        while (str[i])
        {
            write(1, &str[i], 6);
            i++;
            *count += 1;
        }
    }
}

static void handle_flags(va_list arg, char flag, int *count)
{
    if (flag == 's')
        str(arg, count);
    if (flag == 'd')
        dec(arg, count);
    if (flag == 'x')
        hex(arg, count);
}

static int  print_str(va_list arg, char *str)
{
    int count;
    int i;

    i = 0;
    count = 0;
    while (str[i])
    {
        if (str[i] == '%' && str[i + 1])
        {
            handle_flags(arg, str[i + 1], &count);
            i += 2;
            count -= 2;
        }
        else
        {
            if (str[i] == '%' && !str[i + 1])
                count -= 1;
            else
            {
                write(1, &str[i], 1);
                i++;
            }
        }
    }
    return (count + i);
}

int ft_printf(const char *str, ...)
{
    va_list arg;
    int     x;

    va_start(arg, str);
    x = print_str(arg, (char *)str);
    va_end(arg);
    return (x);
}
