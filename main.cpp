#include <bits/stdc++.h>
using namespace std;

/*
 * Complete the 'trade' function below.
 *
 * The function is expected to return a tuple of ints.
 * The function accepts a vector of string records as parameter.
 */
 
using T4 = std::tuple<int, int, int, int>; // price, order_id, qty, self/other
 
 // parse is a function to parse a record and return a vector<string> where elements are like {share, (action, price, qty)...}
 auto parse(const string& record) -> vector<string>{
    vector<string> ret;
    istringstream iss(record);
    string tmp;
    for(;getline(iss, tmp, ' ');)
        ret.push_back(tmp);
    return ret;
 }

std::tuple<int, int, int> trade(vector<string> const& records) {
    constexpr auto SELF = 0;
    constexpr auto OTHER = 1;

    int profit_t = 0, long_exposure = 0, short_exposure = 0;
    int user_id_gen = 1; // increase whenever a new order is pushed into queue and referenced when prices are equal to have FIFO ordering
    
    // 'm' is a map<share, <bid_q, offer_q>> where 'bid_q' is a max-heap and 'offer_q' is a min-heap
    std::unordered_map<string, std::pair<std::priority_queue<T4>, std::priority_queue<T4, vector<T4>, std::greater<T4>>>> m;
    
    for(const auto& rec : records) {
        auto v = parse(rec);
        const int n = v.size();
        const auto& share = v[0]; // symbol such as MAVEN
        auto& [bid_q, offer_q] = m[share]; // corresponding order books
        
        // walkthrough each {action, qty, price}
        for(auto i = 1 ; i < n; i+=3) {
            const auto& action = v[i];
            auto qty_coming = std::stoi(v[i+1]);
            auto price_coming = std::stoi(v[i+2]);
            if(action == "BUY"){
                for(;qty_coming && !offer_q.empty() && std::get<0>(offer_q.top()) <= price_coming ;){
                    auto [price, order_id, qty, user] = offer_q.top();
                    offer_q.pop();
                    const auto min_qty = std::min(qty_coming, qty);
                    qty_coming -= min_qty;
                    if (qty -= min_qty)
                        offer_q.push({price, order_id, qty, user}); // push back the remaining
                    if(user == SELF)
                        short_exposure -= price*min_qty;
                    else // if not self trade, update profit
                        profit_t += (price_coming-price)*min_qty;
                }
                // partial matched
                if(qty_coming){
                    bid_q.push({price_coming, -(user_id_gen++), qty_coming, SELF});
                    long_exposure += price_coming*qty_coming;
                }
            }else if(action == "SELL") {
                for(;qty_coming && !bid_q.empty() && std::get<0>(bid_q.top()) >= price_coming ;){
                    auto [price, order_id, qty, user] = bid_q.top();
                    bid_q.pop();
                    const auto min_qty = std::min(qty_coming, qty);
                    qty_coming -= min_qty;
                    if (qty -= min_qty)
                        bid_q.push({price, order_id, qty, user}); // push back the remaining
                    if(user == SELF)
                        long_exposure -= price*min_qty;
                    else // if not self trade, update profit
                        profit_t += (price-price_coming)*min_qty;
                }
                // partial matched
                if(qty_coming){
                    offer_q.push({price_coming, user_id_gen++, qty_coming, SELF});
                    short_exposure += price_coming*qty_coming;
                }
            }else if(action == "OFFER") {
                for(;qty_coming && !bid_q.empty() && std::get<0>(bid_q.top()) >= price_coming ;){
                    auto [price, order_id, qty, user] = bid_q.top();
                    bid_q.pop();
                    const auto min_qty= std::min(qty_coming, qty);
                    qty_coming -= min_qty;
                    if(qty-=min_qty)
                        bid_q.push({price, order_id, qty, user});
                    if(user == SELF){
                        profit_t += (price-price_coming)*min_qty;
                        long_exposure -= price*min_qty;
                    }
                }
                // partial matched
                if(qty_coming)
                    offer_q.push({price_coming, user_id_gen++, qty_coming, OTHER});
                
            }else if(action == "BID") {
                for(;qty_coming && !offer_q.empty() && std::get<0>(offer_q.top()) <= price_coming ;){
                    auto [price, order_id, qty, user] = offer_q.top();
                    offer_q.pop();
                    auto this_profit = 0;
                    const auto min_qty = std::min(qty_coming, qty);
                    qty_coming -= min_qty;
                    if(qty-=min_qty)
                        offer_q.push({price, order_id, qty, user});
                    if(user == SELF){
                        profit_t += (price_coming-price)*min_qty;
                        short_exposure -= price*min_qty;
                    }
                }
                // partial matched
                if(qty_coming)
                    bid_q.push({price_coming, -(user_id_gen++), qty_coming, OTHER});
            }
        }
    }

    return {profit_t, long_exposure, short_exposure};
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    std::string line;
    std::vector<std::string> records;
    std::getline(std::cin, line);
    while (std::cin)
    {
        records.push_back(line);
        std::getline(std::cin, line);
    }

    auto result = trade(records);
    fout << std::get<0>(result) << ' ';
    fout << std::get<1>(result) << ' ';
    fout << std::get<2>(result) << ' ';
    fout << "\n";

    fout.close();

    return 0;
}

/* Test cases

MAVEN OFFER 10 25
MAVEN BUY 10 20
0 200 0

MAVEN SELL 10 25 BID 10 20
0 0 250

MAVEN BUY 10 20 SELL 5 25 OFFER 10 18 BID 5 28
35 0 0

MAVEN BUY 12 20 SELL 6 25 OFFER 10 18 BID 5 28
35 40 25

MAVEN BUY 12 20 SELL 6 25 SELL 10 18 BUY 5 28
0 40 25

MAVEN BUY 3 20 BID 5 20 BUY 2 20
MAVEN SELL 9 18
10 20 0

TINYCORP SELL 27 1
MAVEN BID 5 20 OFFER 5 25
MEDPHARMA BID 3 120 OFFER 7 150
NEWFIRM BID 10 140 BID 7 150 OFFER 14 180
TINYCORP BID 25 3 OFFER 25 6
FASTAIR BID 21 65 OFFER 35 85
FLYCARS BID 50 80 OFFER 100 90
BIGBANK BID 200 13 OFFER 100 19
REDCHIP BID 55 25 OFFER 80 30
FASTAIR BUY 50 100
CHEMCO SELL 100 67
MAVEN BUY 5 30
REDCHIP SELL 5 30
NEWFIRM BUY 2 200
MEDPHARMA BUY 2 150
BIGBANK SELL 50 11
FLYCARS BUY 200 100
CHEMCO BID 1000 77 OFFER 500 88
2740 11500 152
*/
