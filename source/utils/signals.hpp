

#ifdef _DEBUG
#define _TRACER_ADD()\
	DD::incSlotCount();

#define _TRACER_SUB()\
	DD::decSlotCount();

#else

#define _TRACER_ADD()
#define _TRACER_SUB()

#endif

namespace DD
{
	template<ARG_SIGT>
	class Slot<void(ARG_SIGB)>
	{
	public:
		typedef Slot<void(ARG_SIGB)>* SlotPtr;

	public:
		Slot(int count)
			: count_(count)
			, isNew_(false)			// turn when next trigger
			, prev_(nullptr)
			, next_(nullptr){
			_TRACER_ADD()
		}
		// virtual can be removed.
		virtual ~Slot()	{
			_TRACER_SUB()
		}
		virtual void operator()(ARG_SIGV) = 0;

	public:
		bool isNew_;
		int count_;
		SlotPtr prev_;
		SlotPtr next_;
	private:
		Slot<void(ARG_SIGB)>(const Slot<void(ARG_SIGB)>&);
		Slot<void(ARG_SIGB)>& operator=(const Slot<void(ARG_SIGB)>&);
	};

	template<typename FUNC ARG_SEP ARG_SIGT>
	class FunctorSlot<FUNC, void(ARG_SIGB)>:public Slot<void(ARG_SIGB)>
	{
	public:
		FunctorSlot(FUNC f, int count)
			:func_(f), Slot<void(ARG_SIGB)>(count)
		{

		}
		virtual void operator()(ARG_SIGV)override
		{
			func_(ARG_SIGS);
		}
	private:
		FUNC func_;
	};

	template<ARG_SIGT>
	class Signal<void(ARG_SIGB)>
	{
		typedef Slot<void(ARG_SIGB)>* SlotPtr;
		typedef void* SlotHandle;
	private:
		static void clean(SlotPtr h){
			for(auto ptr=h;ptr;){
				auto k = ptr;
				ptr=ptr->next_;
				delete k;
			}
		}
	public:
		Signal()
			:dep_(0),
			head_(0),
			delete_(0) {
		}


		~Signal() {
			clean(head_);
			clean(delete_);
		}

		template<class T>
		SlotHandle connect(T t, int count=-1) {
			SlotPtr v = new FunctorSlot<T, void(ARG_SIGB)>(t, count);
			if(head_) {
				head_->prev_ = v;
			}
			v->prev_ = nullptr;
			v->next_ = head_;
			head_ = v;
			if(dep_>1){
				v->isNew_ = true;
			}
			return v;
		}

		void trigger(ARG_SIGV) {
			struct _Counter__{
				_Counter__(int &v):v_(v){	++v_;	}
				~_Counter__(){--v_;}
				int &v_;
			}__c(dep_);

			auto now = head_;
			while (now) {
				if (!now->isNew_) {
					(*now)(ARG_SIGS);
				} else {
					now->isNew_ = false;
				}
				--(now->count_);
				if(0==now->count_) {
					now = disconn(now);
				} else {
					now = now->next_;
				}
			}
			if( 1==dep_ ){
				clean(delete_);
				delete_ = nullptr;
			}
		}

	public:
		void disconnect(SlotHandle now)	{
			disconn(reinterpret_cast<SlotPtr>(now));
		}

	private:
		SlotPtr disconn(SlotPtr now) {
			auto rv = now->next_;
			if(head_==now){
				head_ = now->next_;
			}
			if(now->prev_) {
				now->prev_->next_ = now->next_;
			}
			if(now->next_) {
				now->next_->prev_ = now->prev_;
			}
			now->next_ = delete_;
			delete_ = now;	//~onto the top.
			return rv;
		}

	private:
		SlotPtr head_;
		SlotPtr delete_;
		int dep_;
	private:
		Signal<void(ARG_SIGB)>(const Signal<void(ARG_SIGB)>&);
		Signal<void(ARG_SIGB)>& operator=(const Signal<void(ARG_SIGB)>&);
	};
}
