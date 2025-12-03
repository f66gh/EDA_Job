#include<iostream>
using namespace std;

class SignalBuffer {
public:
    SignalBuffer(std::size_t n){
        size_ = n;
        data_ = new double[n]{0};
        mean_valid_ = false;
        mean_cache_ = 0;
        live_count_++;
    };
    
    SignalBuffer() = delete;

    SignalBuffer(std::initializer_list<double> init) : data_(init){}; // 不会了
    // 析构
    ~SignalBuffer(){
        size_ = 0;
        delete[] data_;
        // 这里需不需要加这一行？
        data_ = nullptr;
        live_count_--;
    }
    // 拷贝构造
    SignalBuffer(const SignalBuffer& other): 
    size_(other.size_), 
    data_(other.size_ ? new double[other.size_] : nullptr) {
        for(std::size_t i = 0; i < size_; i++) data_[i] = other.data_[i];
        live_count_++;
    }
    // 拷贝赋值
    SignalBuffer& operator=(const SignalBuffer& other){
        if(this == &other) return *this;

        double* new_data_ = other.size_ ? new double[other.size_] : nullptr;
        for(std::size_t i = 0; i < other.size_; i++) new_data_[i] = other.data_[i];
        delete[] data_;
        data_ = new_data_;
        size_ = other.size_; // 为什么data_要先于size_拷贝？
        mean_valid_ = false;
        return *this;
    }

    // 移动构造
    SignalBuffer(SignalBuffer&& other) noexcept: 
    size_(other.size_),
    data_(other.size_ ? other.data_ : nullptr){
        other.size_ = 0;
        other.data_ = nullptr;
        live_count_++;
    }

    // 移动赋值
    SignalBuffer& operator=(SignalBuffer&& other) noexcept{
        if(this == &other) return *this;

        delete[] data_;

        data_ = other.data_;
        other.data_ = nullptr;

        size_ = other.size_;
        other.size_ = 0;

        mean_valid_ = false;

        return *this;
    }

    std::size_t size() const {
        return size_;
    };

    // 这两个重载接口我应该怎么写能体现加不加const的特点?
    double& operator[](std::size_t i){
        // 假如这个是读取之后归零
        double d = data_[i];
        data_[i] = 0;
        return d;
    };
    // 不懂这是啥意思
    const double& operator[](std::size_t i) const {
        return data_[i];
    };
    double mean() const{
        if(!mean_valid_){
            double sum = 0;
            for(std::size_t i = 0; i < size_; i++){
                sum += data_[i];
            }
            mean_cache_ = sum / size_;
            mean_valid_ = true;
        }
        return mean_cache_;
    }; 

    static std::size_t live_count(); // 为什么静态函数不能用const修饰？

private:
    double* data_ = nullptr;
    std::size_t size_ = 0;
    // 这两个mutable是逻辑const，对外不可修改
    mutable bool mean_valid_ = false;
    mutable double mean_cache_ = 0;
    static std::size_t live_count_;
};

std::size_t live_count_ = 0;
std::size_t live_count(){
    return live_count_;
};

int main(){
    cout<<"Hello, world"<< endl;
    return;
}