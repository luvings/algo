
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <string>
#include <set>
#include <vector>

/* [0, 1) */
#define RANDOM()    (random() / (RAND_MAX + 1.0))

int min_light(std::string road)
{
    if (road.empty()) {
        return 0;
    }

    const char *str = road.c_str();
    int lights = 0;
    uint32_t idx = 0;

    /*
     * 隐含条件: 对于 idx 点, idx 之前的 lights 已经设置好了
     *
     * 1) idx == 'X', idx++
     *
     * 2) idx == '.'
     *      A) (idx + 1) == 'X'
     *          .X?     -> idx += 2, idx set light
     *
     *      B) (idx + 1) == '.'
     *              a) (idx + 2) == 'X'
     *              b) (idx + 2) == '.'
     *              ..X?  ...?      -> idx + 1 set light, idx += 3
     *
     */
    while (idx < road.length()) {
        if (str[idx] == 'X') {
            idx++;
        } else {
            lights++;

            if ((idx + 1) == road.length()) {
                break;
            } else {
                if (str[idx + 1] == 'X') {
                    idx += 2;
                } else {    /* str[idx + 1] == '.' */
                    idx += 3;
                }
            }
        }
    }

    return lights;
}

/*
 * 更简洁的解法
 * 两个 X 之间, 数一下 . 的数量, 然后除以3, 向上取整
 * 把灯数累加
 */
int min_light_2(std::string road)
{
    if (road.empty()) {
        return 0;
    }

    int lights = 0;
    int count = 0;

    for (char c : road) {
        if (c == 'X') {
            lights += (count + 2) / 3;
            count = 0;
        } else {
            count++;
        }
    }

    lights += (count + 2) / 3;

    return lights;
}

/*
 * str[index....]位置, 自由选择放灯还是不放灯
 * str[0..index-1]位置呢? 已经做完决定了, 那些放了灯的位置, 存在lights里
 * 要求选出能照亮所有 . 的方案, 并且在这些有效的方案中, 返回最少需要几个灯
 */
int process_cmp(std::string road, uint32_t idx, std::set < int > &lights)
{
    const char *str = road.c_str();

    if (idx == road.length()) {     /* 结束 */
        for (uint32_t i = 0; i < road.length(); i++) {
            if (str[i] == '.') {
                if (!lights.count(i - 1) && !lights.count(i) && !lights.count(i + 1)) {
                    return INT_MAX;
                }
            }
        }

        return lights.size();
    } else {        /* str 还没结束 */
        /* idx -> X . */
        int no = process_cmp(road, idx + 1, lights);

        int yes = INT_MAX;
        if (str[idx] == '.') {
            lights.insert(idx);

            yes = process_cmp(road, idx + 1, lights);

            lights.erase(idx);
        }

        return std::min(no, yes);
    }
}

int min_light_cmp(std::string road)
{
    if (road.empty()) {
        return 0;
    }

    std::set < int >lights;

    return process_cmp(road.c_str(), 0, lights);
}

std::string create_random_string(int max_length)
{
    int length = (int)(RANDOM() * (max_length + 1));

    std::string road;

    for (int i = 0; i < length; i++) {
        road += RANDOM() < 0.5 ? 'X' : '.';
    }

    return road;
}

int main(int argc, char **argv)
{
    int max_length = 12;
    int times = 5;
    bool succeed = true;

    srandom((unsigned) time(0));

    for (int i = 0; i < times; i++) {
        std::string road = create_random_string(max_length);

        if (min_light(road) != min_light_cmp(road) || min_light_2(road) != min_light_cmp(road)) {
            printf("%s \t\t > min light: %d, %d, %d\n", road.c_str(),
                    min_light(road), min_light_2(road), min_light_cmp(road));
            printf("oops\n");
            succeed = false;
            break;
        }
    }

    printf("result: %s\n", succeed ? "nice" : "fucked");

    return 0;
}
