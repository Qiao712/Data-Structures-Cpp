#include<iostream>
#include<cmath>
using namespace std;

// 递归的-高效的幂运算
int QuickPow1(int base, int n){
    if(n == 0) return 1;
    //if(n == 1) return base;

    if(n&1){    //如果n为奇数
        return QuickPow1(base*base, n/2)*base;
    }else{
        return QuickPow1(base*base, n/2);
    }
}

//非递归的-高效的幂运算
int QuickPow2(int base, int n){
    int pow = 1;
    int tmp = base;
    for(; n>0; n>>=1, tmp *= tmp){
        if(n&1){
            pow *= tmp;
        }
    }
    return pow;
}

//生成前N个自然数的随机序列（不重复） O(N)的算法
int a[1000];
int Rand(int b, int e){
    //生成[b,e]的随机数

}
void seq(int n){
    for(int i = 0; i<n; i++){
        a[i] = i+1;
    }

    for(int i = 0; i<n; i++){
        swap(a[i], a[Rand(0,i)]);
    }
}

//计算n次多项式-秦九韶算法/Horner法则
int A[10000];//储存系数
int Horner(int n, int x){
    int sum = 0;
    for(int i = n; i>=0; i--){
        sum = x*sum + A[i];
    }

    return sum;
}

//素性测试-试除
bool isPrime(int x){
    int sqrt_x = (int)sqrt(x);
    //试除2
    if(x%2 == 0) return false;
    //试除大于2的奇数
    //只需要试除到√N即可。因为如果没有小于等于√N的因数，其不可能有两个大于√N（除自身）的因数。（要是有，两个大于乘起来就大于其本身了）
    for(int i = 3; i <= sqrt_x; i+=2){
        if(x%i == 0) return false;
    }

    return true;
}

//素数筛-埃氏筛法（Eratosthenes筛法）
bool isNotPrime[100000];
void getPrime1(int n){
    for(int i = 0; i<100000; i++) isNotPrime[i] = 0;
    isNotPrime[1] = true;
    
    //用前√n来筛即可，因为一个 非素数 两个（除了1和它本身之外的）因数不可能都大于√n，一定有一个小于等于√n的
    int sqrt_n = (int)sqrt(n);
    for(int i = 1; i<=sqrt_n; i++){
        if(!isNotPrime[i]){
            //筛掉i*i,(i+1)*i,...,(i+j)*i这些含有i素因数i的
            //不用再筛j*i (j<i)的数，因为i之前的数j都被筛过了
            for(int j = i*i; j <= n; j += i) isNotPrime[j] = true;
        }
    }
}

//素数线筛-欧拉筛法
int primes[100000];
int num = 0;
void getPrime2(int n){
    for(int i = 0; i<100000; i++) isNotPrime[i] = 0;
    isNotPrime[1] = true;

    //int sqrt_n = (int)sqrt(n);
    for(int i = 2; i<=n; i++){
        if(!isNotPrime[i]) primes[num++] = i;
        for(int j = 0; j<num; j++){     //用已知的素数乘当前的数i，筛掉后面的
            if(i * primes[j] >= n) break;
            isNotPrime[i * primes[j]] = true;
            if(i % primes[j] == 0) break;   //保证后面的数不被重复筛
            //若i有因数primes[j]的话，在后面的循环中筛掉的i*primes[k](k>j)一定含有更小的素因数primes[j]。
            //而拿掉这个最小的素因数primes[j]后的(i*primes[k])/primes[j] 一定是比i大的（∵primes[k]/primes[j]>1）
            //所以在后面的循环中 i_current * primes[k]一定会被某个更大的i筛掉（这个i就是 (i_current * primes[k])/primes[j])
        }
    }
}