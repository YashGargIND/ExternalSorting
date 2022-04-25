#include<iostream>
#include<vector>
#include<algorithm>
#include<climits>
#define INT_SIZE 4
using namespace std;

class page
{
    public:
        page(int capacity){
            page_size=capacity;
            rec_count=0;
            prev = nullptr;
            next = nullptr;
        };
        page* prev;
        page* next;
        int page_id;
        int rec_count;
        void add_record(int r){
            records.push_back(r);
            rec_count++;
        };
        int get_at_index(int index){
            return records[index];
        };
        void set_at_index(int index,int rec){
            if(index>=records.size())
            {
                cout<<rec<<" "<<index<<"invalid.\n";
                return;
            }
            records[index]=rec;
        };
        bool is_full(){
            if(rec_count==page_size)
            {
                return true;
            }
            return false;
        };
        void sort_page(){
            sort(records.begin(),records.end());
            return;
        };
        virtual ~page (){

        };
    protected:
    private:
        int page_size;
        vector<int>records;

};


class datafile
{
    public:
        datafile(int pages,int rec_per_pg,int recs)
        {
            buffer_pages=pages;
            page_capacity=rec_per_pg;
            total_records=recs;
        }
        void insert_record(int r){
            page* p=header;
            page* pre=p;
            page* q;
            if(header==nullptr)
            {
                p=add_page_in_file(nullptr);
            }
            while(p->is_full())
            {
                pre=p;
                p=p->next;
                if(p==nullptr)
                {
                    break;
                }
            }
            if(p==nullptr)
            {
                p=add_page_in_file(pre);
            }
            p->add_record(r);
            return;
        };
        void print(){
             page* p=header;
            int index=0;
            while(p!=nullptr)
            {
                cout<<index<<": ";
                for(int i=0;i<p->rec_count;i++)
                {
                    cout<<p->get_at_index(i)<<" ";
                }
                cout<<"\n";
                index++;
                p=p->next;
            }
            return;
        };
        void print_status(){
            page* p=header;
            while(p!=nullptr)
            {
                for(int i=0;i<p->rec_count;i++)
                {
                    cout<<p->get_at_index(i)<<"\n";
                }
                p=p->next;
            }
            return;
        };
        void external_sort(){
            create_runs();
            int merging_runs_size=buffer_pages-1;
            int l=0;
            while(runs.size()>1)
            {
                int l=0;
                vector<vector<int> >tmp_runs;
                while(l<runs.size())
                {
                    vector<int>tmp;
                    int ptr[runs.size()]={0};
                    bool new_run_made=false;

                    while(!new_run_made)
                    {
                        int cur_min=INT_MAX, index_used=-1;
                        new_run_made=true;
                        for(int i=l;i<min(l+merging_runs_size,int(runs.size()));i++)
                        {
                            if(ptr[i]<runs[i].size())
                            {
                                new_run_made=false;break;
                            }
                        }
                        for(int i=l;i<min(l+merging_runs_size,int(runs.size()));i++)
                        {
                        //merge runs
                        //store to temporary run.
                            if(ptr[i]>=runs[i].size())
                            {
                                continue;
                            }
                            if(cur_min>=runs[i][ptr[i]])
                            {
                                cur_min=runs[i][ptr[i]];
                                index_used=i;
                            }
                        }
                        if(!new_run_made)tmp.push_back(cur_min);
                        ptr[index_used]++;
                    }
                tmp_runs.push_back(tmp);
                tmp.clear();
                l+=merging_runs_size;
                }
                runs.clear();
                for(int j=0;j<tmp_runs.size();j++)runs.push_back(tmp_runs[j]);
                tmp_runs.clear();
            }
            write_back();
        };
        void create_runs(){
            page* cur=header;
            vector<int>run;
            int f=0;
            while(cur!=nullptr)
            {
                for(int i=0;i<buffer_pages;i++)
                {
                    if(cur==nullptr)
                    {
                        f=1;
                        break;
                    }
                    for(int j=0;j<cur->rec_count;j++)
                    {
                    run.push_back(cur->get_at_index(j));
                    }
                    cur=cur->next;
                }
                sort(run.begin(),run.end());
                runs.push_back(run);
                run.clear();
                if(f==1)break;

            }
        };
        void print_runs(){
            cout<<"\n runs "<<runs.size()<<"\n";
            for(int i=0;i<runs.size();i++)
            {
                cout<<i<<":";
                for(int j=0;j<runs[i].size();j++)
                {
                    cout<<runs[i][j]<<" ";
                }
                cout<<"\n";
            }
        };
        virtual ~datafile(){

        };
    protected:
    private:
        int buffer_pages;
        int page_capacity;
        int total_records;
        page* header;
        page* cur_page;
        page* add_page_in_file(page* previous_node)
        {
            page_count++;
            page* p=new page(page_capacity);
            if(header==nullptr)
            {
                header=p;
                p->prev = nullptr;
                p->next = nullptr;
                p->page_id = 0;
                return p;
            }
            p->prev = previous_node;
            if(previous_node!=nullptr)
            {
                p->next = previous_node->next;
                previous_node->next = p;
                p->page_id = 1+previous_node->page_id;
            }
            if(p->next!=nullptr)
            {
                p->next->prev = p;
            }
            return p;
        };
        int page_count=0;
        void write_back(){
            page* p=header;
            int index=0;
            while(p!=nullptr)
            {
                for(int i=0;i<p->rec_count&&index<runs[0].size();i++)
                {
                    p->set_at_index(i,runs[0][index]);
                    index++;
                }
                p=p->next;
            }
            return;
        };
        vector<vector<int> >runs;
};

int main()
{
    int buffer_pages,records_per_page,total_records;
    cin>>buffer_pages>>records_per_page>>total_records;
    datafile d(buffer_pages,records_per_page,total_records);
    int r;
    for(int i=0;i<total_records;i++)
    {
        cin>>r;
        d.insert_record(r);
    }
    d.external_sort();
    d.print_status();
}
