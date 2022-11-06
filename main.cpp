#include <bits/stdc++.h>
using namespace std;

/*
 * Complete the 'trade' function below.
 *
 * The function is expected to return a tuple of ints.
 * The function accepts a vector of string records as parameter.
 */
 
using T4 = std::tuple<int, int, int, int>; // price, qty, order_id, me/other
 
 // parse is a function to parse record and return a {share, action, price, qty, action, price, qty .....}
 auto parse(const string& record) -> vector<string>{
    vector<string> ret;
    istringstream iss(record);
    string tmp;
    for(;getline(iss, tmp, ' ');)
        ret.push_back(tmp);
    return ret;
 }

std::tuple<int, int, int> trade(vector<string> const& records) {
    constexpr auto ME = 0;
    constexpr auto OTHER = 1;
    int profit_t = 0, long_t = 0, short_t = 0;
    int user_id_gen = 1; // for FIFO when prices are equal
    
    // [share, [bid_q, offer_q]]
    // [share, [max-heap, min-heap]]
    std::unordered_map<string, std::pair<std::priority_queue<T4>, std::priority_queue<T4, vector<T4>, std::greater<T4>>>> m;
    
    for(const auto& rec : records) {
        auto v = parse(rec);
        const int n = v.size();
        const auto& share = v[0];
        auto& [bid_q, offer_q] = m[share];
        for(auto i = 1 ; i < n; i+=3) {
            const auto& action = v[i];
            auto qty_coming = std::stoi(v[i+1]);
            auto price_coming = std::stoi(v[i+2]);
            if(action == "BUY"){
                for(;!offer_q.empty() && std::get<0>(offer_q.top()) <= price_coming && qty_coming;){
                    auto [price, qty, order_id, user] = offer_q.top();
                    offer_q.pop();
                    const auto min_qty = std::min(qty_coming, qty);
                    qty_coming -= min_qty;
                    if (qty -= min_qty)
                        offer_q.push({price, qty, order_id, user}); // push back the remaining
                    if(user == ME)
                        short_t -= price*min_qty;
                    else // if not self trade
                        profit_t += (price_coming-price)*min_qty;
                }
                
                if(qty_coming){
                    bid_q.push({price_coming, qty_coming, -(user_id_gen++), ME});
                    long_t += price_coming*qty_coming;
                }
            }else if(action == "SELL") {
                for(;!bid_q.empty() && std::get<0>(bid_q.top()) >= price_coming && qty_coming;){
                    auto [price, qty, order_id, user] = bid_q.top();
                    bid_q.pop();
                    const auto min_qty = std::min(qty_coming, qty);
                    qty_coming -= min_qty;
                    if (qty -= min_qty)
                        bid_q.push({price, qty, order_id, user}); // push back the remaining
                    if(user == ME)
                        long_t -= price*min_qty;
                    else // if not self trade
                        profit_t += (price-price_coming)*min_qty;
                }
                
                if(qty_coming){
                    offer_q.push({price_coming, qty_coming, user_id_gen++, ME});
                    short_t += price_coming*qty_coming;
                }
                
            }else if(action == "OFFER") {
                for(;!bid_q.empty() && std::get<0>(bid_q.top()) >= price_coming && qty_coming;){
                    auto [price, qty, order_id, user] = bid_q.top();
                    bid_q.pop();
                    const auto min_qty= std::min(qty_coming, qty);
                    qty_coming -= min_qty;
                    if(qty-=min_qty)
                        bid_q.push({price, qty, order_id, user});
                    if(user == ME){
                        profit_t += (price-price_coming)*min_qty;
                        long_t -= price*min_qty;
                    }
                }
                
                if(qty_coming)
                    offer_q.push({price_coming, qty_coming, user_id_gen++, OTHER});
                
            }else if(action == "BID") {
                for(;!offer_q.empty() && std::get<0>(offer_q.top()) <= price_coming && qty_coming;){
                    auto [price, qty, order_id, user] = offer_q.top();
                    offer_q.pop();
                    auto this_profit = 0;
                    const auto min_qty = std::min(qty_coming, qty);
                    qty_coming -= min_qty;
                    if(qty-=min_qty)
                        offer_q.push({price, qty, order_id, user});
                    if(user == ME){
                        profit_t += (price_coming-price)*min_qty;
                        short_t -= price*min_qty;
                    }
                }
                
                if(qty_coming)
                    bid_q.push({price_coming, qty_coming, -(user_id_gen++), OTHER});
            }
        }
    }

    return {profit_t, long_t, short_t};
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
