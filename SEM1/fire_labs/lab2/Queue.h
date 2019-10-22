// TODO: Try to write?
typedef struct 
{
	unsigned int size;
	int* array;
}Queue;


Queue* createQueue(unsigned int size){
	Queue* q = (struct Queue*) malloc(sizeof(struct Queue));
	q->size = size;
	q->array = (int*) malloc(sizeof(int) * queue->size);
	return q;
}

