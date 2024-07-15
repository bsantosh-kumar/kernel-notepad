#include <linux/module.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
struct llnode_t
{
    char *data;
    struct llnode_t *left;
    struct llnode_t *right;
};
typedef struct llnode_t llnode;
int insert_back(llnode **tail, char *data)
{
    char *buff = (char *)kcalloc(200, sizeof(char), GFP_KERNEL);
    int i;
    for (i = 0; i < 200; i++)
        buff[i] = 0;

    llnode *currNode = (llnode *)kmalloc(sizeof(llnode), GFP_KERNEL);
    strcpy(buff, data);
    currNode->data = buff;
    currNode->left = (*tail);
    currNode->right = NULL;
    if ((*tail) != NULL)
        (*tail)->right = currNode;

    (*tail) = currNode;
    return strlen(data);
}
int insert_front(llnode **head, const char *data)
{
    char *buff = (char *)kcalloc(200, sizeof(char), GFP_KERNEL);
    int i;
    for (i = 0; i < 200; i++)
        buff[i] = 0;
    llnode *currNode = (llnode *)kmalloc(sizeof(llnode), GFP_KERNEL);
    strcpy(buff, data);
    currNode->data = buff;
    currNode->right = (*head);
    currNode->left = NULL;
    if ((*head) != NULL)
        (*head)->left = currNode;
    (*head) = currNode;
    return strlen(data);
}

char *pop_front(llnode **head)
{
    if ((*head) == NULL)
        return NULL;
    char *buff = (char *)kcalloc(200, sizeof(char), GFP_KERNEL);
    int i;
    for (i = 0; i < 200; i++)
        buff[i] = 0;
    strcpy(buff, (*head)->data);
    kfree((*head)->data);
    llnode *nextHead = (*head)->right;
    nextHead->left = NULL;
    kfree((*head));
    (*head) = nextHead;
    return buff;
}

char *pop_back(llnode **tail)
{
    if ((*tail) == NULL)
        return NULL;
    char *buff = (char *)kcalloc(200, sizeof(char), GFP_KERNEL);
    int i;
    for (i = 0; i < 200; i++)
        buff[i] = 0;
    strcpy(buff, (*tail)->data);
    kfree((*tail)->data);
    llnode *nextTail = (*tail)->left;
    if (nextTail != NULL)
        nextTail->right = NULL;
    kfree(*tail);
    (*tail) = nextTail;
    return buff;
}
char *print_list(llnode *head)
{
    int count = 1;
    char *ans = (char *)kcalloc(200 * 20, sizeof(char), GFP_KERNEL);
    int i;
    for (i = 0; i < 200 * 20; i++)
        ans[i] = 0;
    while (head != NULL)
    {
        char *temp = (char *)kcalloc(200, sizeof(char), GFP_KERNEL);
        for (i = 0; i < 200; i++)
            temp[i] = 0;
        sprintf(temp, "%d:%s\n", count, head->data);
        strcat(ans, temp);
        count++;
        head = head->right;
        kfree(temp);
    }
    return ans;
}

struct clipboard_storage_t
{
    llnode *head;
    llnode *tail;
    char *currString;
    int size;
};

typedef struct clipboard_storage_t clipboard_storage;
void init_clipboard(clipboard_storage *c)
{
    c->head = NULL;
    c->tail = NULL;
    char *currString = NULL;
    c->size = 0;
    printk("Intialised\n");
}

void add_to_clipboard(clipboard_storage *c, const char *data)
{
    insert_front(&(c->head), data);
    if (c->tail == NULL)
        c->tail = c->head;
    c->currString = c->head->data;
    c->size++;
    printk("size=%d currString=%s\n", c->size, c->currString);
}
char *remove_from_clipboard(clipboard_storage *c)
{
    char *buff = pop_back(&c->tail);
    if (c->tail == NULL)
    {
        c->head = NULL;
        c->currString = NULL;
    }
    else
    {
        c->currString = c->head->data;
    }
    if (c->size > 0)
        c->size--;
    return buff;
}
int change_currString(clipboard_storage *c, int n)
{
    if (n <= 0)
        return -1;
    llnode *t = c->head;
    if (n > c->size)
    {
        if ((c->head) != NULL)
            c->currString = c->head->data;
        else
            c->currString = NULL;
        return -1;
    }
    n--;
    while (n--)
    {
        t = t->right;
    }
    c->currString = t->data;
    return strlen(c->currString);
}
void print_clipboard(clipboard_storage *c)
{
    char *list = print_list(c->head);
    printk("Curr clipboard is:%s\n", list);
    kfree(list);
}

clipboard_storage *clip;

int opendev(struct inode *i, struct file *f);
int closedev(struct inode *i, struct file *f);
ssize_t readme(struct file *f, char *c, size_t t, loff_t *o);
ssize_t writeme(struct file *f, const char *c, size_t t, loff_t *o);
int Major;
#define SUCCESS 0
#define DEVICE_NAME "LinuXCliPBoarD"
static struct file_operations fops = {
    .read = readme,
    .write = writeme,
    .open = opendev,
    .release = closedev};

int device_init(void)
{
    Major = register_chrdev(0, DEVICE_NAME, &fops);
    if (Major > 0)
    {
        printk("Clipboard is Ready For Saving Data!:%d\n", Major);
        clip = (clipboard_storage *)kmalloc(sizeof(clipboard_storage), GFP_KERNEL);
        init_clipboard((clip));
        return 0;
    }
    else
    {
        printk("Something Bad happened OhOhhh: %d", Major);
        return -1;
    }
}
module_init(device_init);

void device_exit(void)
{
    unregister_chrdev(Major, DEVICE_NAME);
    printk("Goodbye Crewl World!\n");
}
module_exit(device_exit);

MODULE_LICENSE("GPL");

int opendev(struct inode *a, struct file *b)
{
    printk("Someone opened me!\n");
    //nothing to do!!!
    return SUCCESS;
}
int closedev(struct inode *a, struct file *b)
{
    //nothing to do!!
    printk("Someone Closed me!\n");
    return SUCCESS;
}

void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
        swap(*(str + start), *(str + end));
        start++;
        end--;
    }
}
char *b;
// int pow(int a, int b);
int itoa(int num, char *str)
{
    int base = 10;
    int i = 0;
    bool isNegative = false;

    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        int x = strlen(str);
        strcpy(b, str);
        return x;
    }

    // In standard itoa(), negative numbers are handled only with
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0'; // Append string terminator

    // Reverse the string
    reverse(str, i);

    int y = strlen(str);
    strcpy(b, str);
    return y + 1;
}
int atoi(const char *str)
{
    int res = 0, i = 0; // Initialize result

    // Iterate through all characters of input string and
    // update result
    for (i = 0; str[i] == ' ' || str[i] == '\t' || str[i] == '\n'; i++)
    {
    }

    for (i; str[i] != '\0' && str[i] != ' ' && str[i] != '\t' && str[i] != '\n'; ++i)
    {
        if (str[i] >= '0' && str[i] <= '9')
            res = res * 10 + str[i] - '0';
        else
            return -1;
    }

    // return result.

    return res;
}

ssize_t readme(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    int len;
    if (clip->currString != NULL)
    {
        if (copy_to_user(buffer, clip->currString, strlen(clip->currString) + 1) != 0)
            return -EFAULT;
        printk("Top is %s\n", clip->currString);
        return strlen(clip->currString);
    }
    else
    {
        char buff[200] = {0};
        strcpy(buff, "<No string>\0");
        printk("Clip board is empty\n");
        len = strlen(buff);
        if (copy_to_user(buffer, buff, len + 1) != 0)
            return -EFAULT;
        return strlen(buff);
    }
}

bool prefix(const char *str, const char *pre)
{
    return strncmp(pre, str, strlen(pre)) == 0;
}
ssize_t writeme(struct file *filp, const char *buff, size_t len, loff_t *off)
{
    if (prefix(buff, "!print"))
    {
        print_clipboard(clip);
        return clip->size;
    }
    else if (prefix(buff, "!change"))
    {
        const char *num_str = buff + 7;
        printk("%s\n", num_str);
        int num = atoi(num_str);
        printk("num=%d\n", num);
        int returnVal = change_currString(clip, num);
        if (clip->currString)
        {
            printk("Changed head to %s\n", clip->currString);
            return strlen(clip->currString);
        }
        return returnVal;
    }
    else if (prefix(buff, "!add"))
    {
        const char *new_str = buff + 4;
        add_to_clipboard(clip, new_str);
        printk("Added %s to clip\n", new_str);
        return strlen(new_str);
    }
    else if (prefix(buff, "!popback"))
    {
        char *removed_str = remove_from_clipboard(clip);
        printk("Removed %s\n", removed_str);
        if (removed_str != NULL)
        {
            int len = strlen(removed_str);
            kfree(removed_str);
            return len;
        }
        else
        {
            return -1;
        }
    }
    return clip->size;
}