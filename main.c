#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// sytax tree
struct st {
    char *tag;
    char *content;
    struct st *kids;
    struct st *siblings;
};

char *compile(char *src);
char *concat_prints(char *src);
void append_child(struct st* child, struct st* parent);
struct st *parse_tags(char *src);
void parse_tags_section(struct st* parent, char *src);
void truc_str(char *out, char *str, int len);
void substring(char *out, char *str, int start, int end);
int i_of(char *str, char *src);
int i_of_from(char *str, int start, char *src);
int i_of_closing_tag(int start, char *src);

int main(int argc, char **argv)
{
    //printf("Hello World");
    //printf("%d\n", i_of_closing_tag(10, "<u> hello <b> Planet </b> </u>"));
    //st root;
    //parse_tags(&root, "Hello <b>dude</b>");
    char *fog_program = "This is a fog program that prints out the name of the program...\
    <u>I am <b>program</b></u>\
    and then the version number\
    <u>version <b>version number</b></u>";
    struct st *syntax_tree = parse_tags(fog_program);

    printf("tag: \"%s\" content: \"%s\"\n", syntax_tree->tag,
                                            syntax_tree->content);
    printf("\ttag: \"%s\" content: \"%s\"\n", syntax_tree->kids->tag,
                                            syntax_tree->kids->content);
    printf("\ttag: \"%s\" content: \"%s\"\n", syntax_tree->kids->siblings->tag,
                                            syntax_tree->kids->siblings->content);
    printf("\t\ttag: \"%s\" content: \"%s\"\n", syntax_tree->kids->siblings->kids->tag,
                                                syntax_tree->kids->siblings->kids->content);
    printf("\t\ttag: \"%s\" content: \"%s\"\n", syntax_tree->kids->siblings->kids->siblings->tag,
                                                syntax_tree->kids->siblings->kids->siblings->content);
    printf("\ttag: \"%s\" content: \"%s\"\n", syntax_tree->kids->siblings->siblings->tag,
                                            syntax_tree->kids->siblings->siblings->content);
    printf("\ttag: \"%s\" content: \"%s\"\n", syntax_tree->kids->siblings->siblings->siblings->tag,
                                            syntax_tree->kids->siblings->siblings->siblings->content);
    printf("\t\ttag: \"%s\" content: \"%s\"\n", syntax_tree->kids->siblings->siblings->siblings->kids->tag,
                                                syntax_tree->kids->siblings->siblings->siblings->kids->content);
    printf("\t\ttag: \"%s\" content: \"%s\"\n", syntax_tree->kids->siblings->siblings->siblings->kids->siblings->tag,
                                                syntax_tree->kids->siblings->siblings->siblings->kids->siblings->content);

}


char *compile(char *src)
{
    return "";
}

/*
 * optimizes number of print statements for sake of transpiled output readablitiy
 */
char *concat_prints(char *src)
{
    return "";
}

void append_child(struct st* child, struct st* parent) {
    if (parent->kids == NULL) {
        parent->kids = child;
    } else {
        struct st *curr_child = parent->kids;
        while (curr_child->siblings != NULL) curr_child = curr_child->siblings;
        curr_child->siblings = child;
    }
}

struct st *parse_tags(char *src)
{
    struct st *root = malloc(sizeof(struct st));
    root->tag = "fog";
    root->content = src;
    parse_tags_section(root, src);
    return root;
}

// src should always be the content of parent
void parse_tags_section(struct st* parent, char *src)
{
    int i = 0;
    while (i < strlen(src)) {
        struct st *child = malloc(sizeof(struct st));

        if (src[i] == '<') {
            int openingTagStart = i;
            int openingTagEnd = i_of_from(">", openingTagStart, src);
            int closingTagStart = i_of_closing_tag(openingTagEnd, src);
            int closingTagEnd = i_of_from(">", closingTagStart, src);

            //char tagElement[elementEnd - elementStart];
            //substring(tagElement, src, elementStart, elementEnd);
            //node->tag = tagElement;
            int elementStart = openingTagStart + 1;
            int elementEnd = i_of_from(" ", openingTagStart, src);
            elementEnd = (elementEnd < 0) ? openingTagEnd : MIN(elementEnd, openingTagEnd);
            child->tag = malloc(sizeof(char) * (elementEnd - elementStart));
            substring(child->tag, src, elementStart, elementEnd);

            int contentStart = openingTagEnd + 1;
            int contentEnd = closingTagStart;
            child->content = malloc(sizeof(char) * (contentEnd - contentStart));
            substring(child->content, src, contentStart, contentEnd);

            parse_tags_section(child, child->content);

            i = closingTagEnd + 1;
        } else {
            child->tag = "no_tag"; //temporary

            int contentStart = i;
            int contentEnd = i_of_from("<", i, src);
            contentEnd = (contentEnd < 0) ? strlen(src) : contentEnd;
            child->content = malloc(sizeof(char) * (contentEnd - contentStart));
            substring(child->content, src, contentStart, contentEnd);

            i = contentEnd;
        }
        append_child(child, parent);
    }
}

void trunc_str(char *out, char *str, int len)
{
    strncpy(out, str, len);
    out[len] = '\0';
}

void substring(char *out, char *str, int start, int end)
{
    trunc_str(out, str + start, end - start);
}

int i_of(char *str, char *src)
{
    return strstr(src, str) - src;
}

int i_of_from(char *str, int start, char *src)
{
    return strstr(src + start, str) - src;
}

int i_of_closing_tag(int start, char *src)
{
    int depth = 0;
    for (int i = start+1; i < strlen(src); i++) {
        if (src[i] == '<') {
            if (src[i+1] == '/') {
                if (depth == 0) return i;
                depth -= 1;
            } else {
                depth += 1;
            }
        }
    }
    return -1;
}
