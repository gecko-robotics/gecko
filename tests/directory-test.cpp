#include "core.hpp"
#include <stdio.h>

using namespace std;

int main(){
    // doesn't work
    // Directory d = {
    //     {"bob", {{"a","tcp://a"}, {"b","tcp://b"}}},
    //     {"tob", {{"a","tcp://a"}, {"b","tcp://b"}}}
    // };

    Directory d;
    d.push("bob", "a", "tcp://a");
    d.push("bob", "b", "tcp://b");

    d.push("tom", "a", "tcp://a");
    d.print();

    printf("find[bob][b]: %s\n", d.find("bob", "b").c_str());
    printf("Dir: %d keys  bob: %d   tom: %d\n", d.numberKeys(), d.numberTopics("bob"), d.numberTopics("tom"));
}
