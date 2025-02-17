#include <unistd.h> /* execl */
#include <stdlib.h> /* fork */
#include <sys/wait.h> /* wait, WEXITSTATUS */
#include <stdio.h> /* printf */

int main(void) {
    if (fork() == 0) {
        /* Call false. */
        execl("./false", "./false", (char *)NULL);
    }
    int status;
    /* Wait for a child to finish. */
    wait(&status);
    /* Status encodes multiple fields,
     * we need WEXITSTATUS to get the exit status:
     * http://stackoverflow.com/questions/3659616/returning-exit-code-from-child
     **/
    printf("$? = %d\n", WEXITSTATUS(status));
}
