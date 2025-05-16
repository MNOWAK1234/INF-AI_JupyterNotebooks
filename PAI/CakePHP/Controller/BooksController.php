<?php
namespace App\Controller;

use App\Controller\AppController;

class BooksController extends AppController
{
    public function initialize(): void
    {
        parent::initialize();
        $this->loadComponent('Paginator');
        $this->loadComponent('Flash');
    }
    public function index()
    {
        $books = $this->Paginator->paginate($this->Books->find());
        $this->set(compact('books'));
    }
    public function view($id)
    {
        $book = $this->Books->findById($id)->firstOrFail();
        $this->set(compact('book'));
    }
    public function add()
    {
        $book = $this->Books->newEmptyEntity();
        if ($this->request->is('post')) {
            $book = $this->Books->patchEntity(
                $book,
                $this->request->getData()
            );
            if ($this->Books->save($book)) {
                $this->Flash->success(__('Your book has been added'));
                return $this->redirect(['action' => 'index']);
            }
            $this->Flash->error(__('Unable to add book.'));
        }
        $this->set('book', $book);
    }
    public function delete($id)
    {
        $book = $this->Books->findById($id)->firstOrFail();
        if ($this->Books->delete($book)) {
            $this->Flash->success(
                __(
                    'The {0} book has been deleted.',
                    $book->title
                )
            );
            return $this->redirect(['action' => 'index']);
        }
    }
    public function edit($id)
    {
        $book = $this->Books->findById($id)->firstOrFail();
        if ($this->request->is(['post', 'put'])) {
            $this->Books->patchEntity(
                $book,
                $this->request->getData()
            );
            if ($this->Books->save($book)) {
                $this->Flash->success(__('Your book has been updated.'));
                return $this->redirect(['action' => 'index']);
            }
            $this->Flash->error(__('Unable to update your book.'));
        }
        $this->set('book', $book);
    }
}
