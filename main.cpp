#include <vector>
#include <QVector>
#include <boost/container/vector.hpp>
#include <boost/algorithm/string.hpp>
#include <benchmark/benchmark.h>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/random.hpp>

using namespace std;
using namespace benchmark;

#define ELEMENT_COUNT 1024*1024*256
#define INTEGER_NUMBER_COUNT 1024*1024*128
#define DOUBLE_NUMBER_COUNT 1024*1024*128

void BM_STDVectorPushbackBenchmark(State &state)
{
    vector<int> vec;

    int count = state.range(0);
    for(auto _ : state)
    {
        for(int i=0;i<count;++i)
            vec.push_back(i);
    }
    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(state.range(0)));
}

//BENCHMARK(BM_STDVectorPushbackBenchmark)->DenseRange(0,ELEMENT_COUNT,1024*1024*128);

void BM_QtVectorPushbackBenchmark(State &state)
{
    QVector<int> vec;
    int count = state.range(0);
    for(auto _ : state)
        for(int i=0;i<count;++i)
            vec.push_back(i);
    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(state.range(0)));
}

//BENCHMARK(BM_QtVectorPushbackBenchmark)->DenseRange(0,ELEMENT_COUNT,1024*1024*128);

void BM_BoostVectorPushbackBenchmark(State &state)
{
    boost::container::vector<int> vec;

    int count = state.range(0);
    for(auto _ : state)
        for(int i=0;i<count;++i)
            vec.push_back(i);
    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(state.range(0)));
}
//BENCHMARK(BM_BoostVectorPushbackBenchmark)->DenseRange(0,ELEMENT_COUNT,1024*1024*128);

void BM_STDVectorInsertBenchmark(State &state)
{
    int count = state.range(0);
    vector<int> vec;
    for(auto _ : state)
        for(int i=0;i<count;++i)
            vec.insert(vec.begin(),i);
    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(state.range(0)));
}
//BENCHMARK(BM_STDVectorInsertBenchmark)->DenseRange(0,1024*2,128);

void BM_QVectorInsertBenchmark(State &state)
{
    int count = state.range(0);
    QVector<int> vec;
    for(auto _ : state)
        for(int i=0;i<count;++i)
            vec.insert(vec.begin(),i);
    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(state.range(0)));
}
//BENCHMARK(BM_QVectorInsertBenchmark)->DenseRange(0,1024*2,128);

void BM_BoostVectorInsertBenchmark(State &state)
{
    int count = state.range(0);
    boost::container::vector<int> vec;
    for(auto _ : state)
        for(int i=0;i<count;++i)
            vec.insert(vec.begin(),i);
    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(state.range(0)));
}
//BENCHMARK(BM_BoostVectorInsertBenchmark)->DenseRange(0,1024*2,128);

std::set<int64_t> ConstructRandomSet(int64_t size) {
    std::set<int64_t> s;
    for (int i = 0; i < size; ++i) s.insert(s.end(), i);
    return s;
}

static void BM_SetInsert(benchmark::State& state) {
    std::set<int64_t> data;
    for (auto _ : state) {
        state.PauseTiming();
        data = ConstructRandomSet(state.range(0));
        state.ResumeTiming();
        for (int j = 0; j < state.range(1); ++j)
            data.insert(rand()%state.range(1));
    }
    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(state.range(0)));
}
//BENCHMARK(BM_SetInsert)
//    ->Args({1<<10, 128})
//    ->Args({2<<10, 128})
//    ->Args({4<<10, 128})
//    ->Args({8<<10, 128})
//    ->Args({1<<10, 512})
//    ->Args({2<<10, 512})
//    ->Args({4<<10, 512})
//    ->Args({8<<10, 512});

void BM_STDStringAppend(benchmark::State& state)
{
    string str;
    for(auto _ : state)
    {
        int count = state.range(0);
        for(int i=0;i<count;++i)
        {
            str.append("a");
        }
    }
    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(state.range(0)));
}
//BENCHMARK(BM_STDStringAppend)->DenseRange(0,1024*1024*128,1024*128*32);

void BM_QStringAppend(benchmark::State& state)
{
    QString str;
    for(auto _ : state)
    {
        int count = state.range(0);
        for(int i=0;i<count;++i)
        {
            str.append("a");
        }
    }
    state.SetBytesProcessed(int64_t(state.iterations()) *
                            int64_t(state.range(0)));
}
//BENCHMARK(BM_QStringAppend)->DenseRange(0,1024*1024*128,1024*128*32);

void BM_Memcpy(benchmark::State& state)
{
    char *src_mem = nullptr;
    char *dst_mem = nullptr;
    for(auto _ : state)
    {
        src_mem = new char[state.range(0)];
        dst_mem = new char[state.range(0)];
        memset(src_mem,0,sizeof(char)*state.range(0));
        memcpy(dst_mem,src_mem,sizeof(char)*state.range(0));

        delete []src_mem;
        delete []dst_mem;
    }
    state.SetBytesProcessed(int64_t(state.iterations()) *
                                     int64_t(state.range(0)));
}
//BENCHMARK(BM_Memcpy)->DenseRange(1024,1024*1024,1024);


std::array<int,INTEGER_NUMBER_COUNT> s;
void GenerateRandomData()
{
//    FILE *integerdatafile = fopen("./BanchmarkDataOfSort_Random_INTEGER.bin","wb");
//    FILE *doubledatafile = fopen("./BanchmarkDataOfSort_Rnadom_DOUBLE.bin","rb");
    std::time_t now = std::time(0);
    boost::random::mt19937 gen{static_cast<std::uint32_t>(now)};
    boost::random::uniform_int_distribution<> dist{1, INTEGER_NUMBER_COUNT};
    int count = INTEGER_NUMBER_COUNT;
//    fwrite(&count,sizeof(int),1,integerdatafile);
    int num;
    for(int i=0;i<INTEGER_NUMBER_COUNT;++i)
    {
       // num = dist(gen);
//        fwrite(&num,sizeof(int),1,integerdatafile);
        s[i] = dist(gen);
    }


//    fclose(integerdatafile);
//    fclose(doubledatafile);

}

void BM_STDSort(benchmark::State& state)
{
    for(auto _ : state)
    {
        GenerateRandomData();
        std::sort(s.begin(),s.end());
    }

    state.SetBytesProcessed(int64_t(state.iterations()));
}
BENCHMARK(BM_STDSort);


BENCHMARK_MAIN();

//int main(int argc, char *argv[])
//{
//    QCoreApplication a(argc, argv);

//    return a.exec();
//}
