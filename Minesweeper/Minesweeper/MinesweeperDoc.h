
// MinesweeperDoc.h : интерфейс класса CMinesweeperDoc
//


#pragma once

class CMinesweeperDoc : public CDocument
{
//protected:
public:
	int* m_field;        //данные о поле
	int* m_userField;   //данные об известной пользователю части поля
	int m_sizeX;
	int m_sizeY;
	int m_bombs;
	int m_allBombs;
	int game_start; //0 - игра не началась, 1 - игра началась
	
public:
	int* getField() { return m_field; }
	int* getUserField() { return m_userField; }
	int getSizeX() { return m_sizeX; }
	int getSizeY() { return m_sizeY; }
	void setSizeX(int a) { m_sizeX=a; }
	void setSizeY(int a) { m_sizeY=a; }
	void set_game_start(int status) { game_start = status; }
	void Draw(CDC *PDC);
	int check_win();
	void boom();
	void win();
	void setDifficulty(int dif); 
	int getStart() { return game_start; }
	void DrawCell(CDC *PDC, int x, int y);
	void initField(int x, int y, int bombs, bool images);
protected: // создать только из сериализации
	CMinesweeperDoc();
	DECLARE_DYNCREATE(CMinesweeperDoc)

// Атрибуты
public:

// Операции
public:

// Переопределение
public:
	virtual void Serialize(CArchive& ar);

#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CMinesweeperDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnDifficulty1();
	afx_msg void OnUpdateDifficulty1(CCmdUI *pCmdUI);
	afx_msg void OnDifficulty2();
	afx_msg void OnUpdateDifficulty2(CCmdUI *pCmdUI);
	afx_msg void OnDifficulty3();
	afx_msg void OnDifficulty4();
	afx_msg void OnUpdateDifficulty3(CCmdUI *pCmdUI);
	//afx_msg void OnConnectiontogame();
};
