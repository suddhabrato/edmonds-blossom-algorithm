#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <bits/stdc++.h>
using namespace std;

#include <GL/glut.h>

#define WW 1280
#define WH 720
#define M 500

void gpInit();
void gpMouse(int, int, int, int);
void drawPoint(int, int, string);
void drawLine(int, int, int, int, string);
void drawblossompoint(int, int, int);
void drawsmallpoint(int, int, int);

struct StructEdge
{
    int vertex;
    StructEdge *next;
};

typedef StructEdge *Edge;

FILE *fp, *fpin, *fpout;
double r = 0.0, g = 0.0, b = 0.0, a = 0.0;
int click = 0, xa = -1, ya = -1, xb = -1, yb = -1, radius = 20, Vnum = 0, u = 0, v = 0;
map<int, pair<int, int>> vertices;
vector<pair<int, int>> edges;
int activeVertex = -1, originalVertex = -1, startVertex = -1, bmnode = -1;
bool inputphase = true, flag = false;

class Blossom
{
    StructEdge pool[M * M * 2];
    Edge top = pool, adj[M];
    int V, E, q_front, q_rear;
    int match[M], bfs_queue[M], father[M], base[M];
    bool inq[M], inb[M], ed[M][M];

public:
    Blossom(int V, int E) : V(V), E(E) {}

    void addEdge(int u, int v)
    {
        if (!ed[u][v] && u != v)
        {
            top->vertex = v;
            top->next = adj[u];
            adj[u] = top++;

            top->vertex = u;
            top->next = adj[v];
            adj[v] = top++;

            ed[u][v] = ed[v][u] = true;
        }
    }

    // Finds Lowest common ancestor of u and v
    int LCA(int root, int u, int v)
    {
        static bool inp[M];
        memset(inp, 0, sizeof(inp));
        while (1)
        {
            inp[u = base[u]] = true;
            if (u == root)
                break;
            u = father[match[u]];
        }
        while (1)
        {
            if (inp[v = base[v]])
                return v;
            else
                v = father[match[v]];
        }
    }

    void mark_blossom(int lca, int u)
    {
        while (base[u] != lca)
        {
            int v = match[u];
            inb[base[u]] = inb[base[v]] = true;
            u = father[v];
            if (base[u] != lca)
                father[u] = v;
            updateGraph();
            pausefunc();
        }
    }

    void blossom_contraction(int s, int u, int v)
    {
        int lca = LCA(s, u, v);
        bmnode = lca;
        updateGraph();
        pausefunc();
        memset(inb, 0, sizeof(inb));
        mark_blossom(lca, u);
        mark_blossom(lca, v);
        if (base[u] != lca)
            father[u] = v;
        if (base[v] != lca)
            father[v] = u;
        for (int u = 0; u < V; u++)
            if (inb[base[u]])
            {
                base[u] = lca;
                if (!inq[u])
                    inq[bfs_queue[++q_rear] = u] = true;
            }
    }

    void pausefunc()
    {
        flag = getc(stdin);
    }

    int find_augmenting_path(int s)
    {
        originalVertex = s;
        startVertex = -1;
        activeVertex = -1;

        updateGraph();
        pausefunc();

        memset(inq, 0, sizeof(inq));
        memset(father, -1, sizeof(father));
        for (int i = 0; i < V; i++)
            base[i] = i;
        inq[bfs_queue[q_front = q_rear = 0] = s] = true;
        while (q_front <= q_rear)
        {
            int u = bfs_queue[q_front++];
            startVertex = u;
            activeVertex = -1;
            updateGraph();
            pausefunc();

            for (Edge e = adj[u]; e; e = e->next)
            {
                int v = e->vertex;
                activeVertex = v;
                updateGraph();
                pausefunc();
                if (base[u] != base[v] && match[u] != v)
                    if ((v == s) || (match[v] != -1 && father[match[v]] != -1))
                    {
                        blossom_contraction(s, u, v);
                        updateGraph();
                        pausefunc();
                    }
                    else if (father[v] == -1)
                    {
                        father[v] = u;
                        updateGraph();
                        pausefunc();
                        if (match[v] == -1)
                            return v;
                        else if (!inq[match[v]])
                            inq[bfs_queue[++q_rear] = match[v]] = true;
                    }
            }
        }
        return -1;
    }

    int augment_path(int s, int t)
    {
        int u = t, v, w;
        while (u != -1)
        {
            v = father[u];
            w = match[v];
            match[v] = u;
            match[u] = v;
            u = w;
            updateGraph();
            pausefunc();
        }
        return t != -1;
    }

    void updateGraph()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        for (auto checkedge : edges)
        {
            int v1 = checkedge.first;
            int v2 = checkedge.second;
            xa = vertices[v1].first;
            ya = vertices[v1].second;
            xb = vertices[v2].first;
            yb = vertices[v2].second;
            if (match[v1 - 1] == (v2 - 1) && match[v2 - 1] == (v1 - 1))
                drawLine(xa, ya, xb, yb, "red");
            else
                drawLine(xa, ya, xb, yb, "black");
        }
        for (auto checkv : vertices)
        {
            int ve = checkv.first;
            if (bmnode != -1 && bmnode == (ve - 1))
                drawblossompoint(vertices[ve].first, vertices[ve].second, radius + 20);
            if (inb[ve - 1])
                drawblossompoint(vertices[ve].first, vertices[ve].second, radius + 10);
            if (originalVertex != -1 && (ve - 1) == originalVertex)
                drawPoint(vertices[ve].first, vertices[ve].second, "green");
            if (startVertex != -1 && (ve - 1) == startVertex)
                drawPoint(vertices[ve].first, vertices[ve].second, "blue");
            if (activeVertex != -1 && (ve - 1) == activeVertex)
                drawPoint(vertices[ve].first, vertices[ve].second, "red");
            if (((ve - 1) != originalVertex) && ((ve - 1) != startVertex) && ((ve - 1) != activeVertex))
                drawPoint(vertices[ve].first, vertices[ve].second, "gray");
        }
        if (activeVertex != -1 && father[activeVertex] != -1)
            drawsmallpoint(vertices[father[activeVertex] + 1].first, vertices[father[activeVertex] + 1].second, radius / 2);
        glFlush();
    }

    int edmondsBlossomAlgorithm()
    {
        int match_counts = 0;
        memset(match, -1, sizeof(match));
        for (int u = 0; u < V; u++)
            if (match[u] == -1)
                match_counts += augment_path(u, find_augmenting_path(u));

        activeVertex = originalVertex = startVertex = -1, bmnode = -1;
        memset(inb, 0, sizeof(inb));
        updateGraph();
        return match_counts;
    }
    void printMatching(int maxMatch)
    {
        rewind(fpout);
        fprintf(fpout, "%d\n", maxMatch);
        for (int i = 0; i < V; i++)
            if (i < match[i])
                fprintf(fpout, "%d %d\n", i + 1, match[i] + 1);
    }
};
void edmondcall()
{
    fpin = fopen("input.txt", "r+");
    fpout = fopen("output.txt", "w+");
    int u, v;
    int V, E;
    rewind(fpin);
    fscanf(fpin, "%d %d", &V, &E);
    Blossom bm(V, E);
    while (E--)
    {
        fscanf(fpin, "%d %d", &u, &v);
        bm.addEdge(u - 1, v - 1);
    }
    int res = bm.edmondsBlossomAlgorithm();
    bm.printMatching(res);
    fclose(fpin);
    fclose(fpout);
    cout << "Press Enter thrice to quit\n";
    flag = getc(stdin);
    flag = getc(stdin);
    flag = getc(stdin);
    exit(0);
}
void display()
{
    fprintf(fp, "%d %d\n", vertices.size(), edges.size());
    sort(edges.begin(), edges.end());
    for (auto x : edges)
    {
        fprintf(fp, "%d %d\n", x.first, x.second);
    }
    fclose(fp);
    edmondcall();
}
int main(int argc, char *argv[])
{
    fp = fopen("input.txt", "w+");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(WW, WH);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Edmond's Blossom Algorithm");
    glutMouseFunc(gpMouse);
    gpInit();
    glutMainLoop();
    return 0;
}

void gpInit()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glColor4f(0.0, 0.0, 0.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, WW, 0, WH);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();
}

int existVertex(int xp, int yp)
{
    for (auto q : vertices)
    {
        int xcentre = q.second.first;
        int ycentre = q.second.second;
        if (((xcentre - xp) * (xcentre - xp) + (ycentre - yp) * (ycentre - yp)) < radius * radius)
            return q.first;
    }
    return -1;
}

void gpMouse(int button, int state, int x, int y)
{
    y = glutGet(GLUT_WINDOW_HEIGHT) - y;
    if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
    {
        click++;
        if (inputphase)
        {
            if (click % 2 != 0)
            {

                int val = existVertex(x, y);
                if (val == -1)
                {
                    Vnum++;
                    u = Vnum;
                    vertices[Vnum] = {x, y};
                    drawPoint(x, y, "gray");
                }
                else
                {
                    u = val;
                    x = vertices[val].first;
                    y = vertices[val].second;
                }
                xa = x;
                ya = y;
            }
            else
            {
                int val = existVertex(x, y);
                if (val == -1)
                {
                    Vnum++;
                    v = Vnum;
                    vertices[Vnum] = {x, y};
                    drawPoint(x, y, "gray");
                }
                else
                {
                    x = vertices[val].first;
                    y = vertices[val].second;
                    v = val;
                }
                xb = x;
                yb = y;
                drawLine(xa, ya, xb, yb, "black");
                drawPoint(xa, ya, "gray");
                drawPoint(xb, yb, "gray");
                edges.push_back({u, v});
            }
        }
    }
    if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN))
    {
        if (inputphase)
        {
            inputphase = false;
            glFlush();
            display();
        }
    }
    glFlush();
    return;
}

void drawPoint(int x, int y, string col)
{
    if (col == "light")
    {
        r = 0.0;
        g = 1.0;
        b = 0.3;
        a = 0.5;
    }
    if (col == "red")
    {
        r = 1.0;
        g = 0.0;
        b = 0.0;
        a = 1.0;
    }
    if (col == "black")
    {
        r = 0.0;
        g = 0.0;
        b = 0.0;
        a = 1.0;
    }
    if (col == "gray")
    {
        r = 0.5;
        g = 0.5;
        b = 0.5;
        a = 1.0;
    }
    if (col == "blue")
    {
        r = 0.0;
        g = 0.0;
        b = 1.0;
        a = 1.0;
    }
    if (col == "green")
    {
        r = 0.0;
        g = 1.0;
        b = 0.0;
        a = 1.0;
    }
    glColor4f(r, g, b, a);
    glPointSize((double)radius);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void drawblossompoint(int x, int y, int rad)
{
    glColor4f(1.0, 1.0, 0.0, 0.5);
    glPointSize((double)rad);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void drawsmallpoint(int x, int y, int rad)
{
    glColor4f(1.0, 0.5, 0.0, 0.5);
    glPointSize((double)rad);
    glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

void drawLine(int xa, int ya, int xb, int yb, string col)
{
    if (col == "red")
    {
        r = 1.0;
        g = 0.0;
        b = 0.0;
        a = 1.0;
    }
    if (col == "black")
    {
        r = 0.0;
        g = 0.0;
        b = 0.0;
        a = 1.0;
    }
    if (col == "gray")
    {
        r = 0.5;
        g = 0.5;
        b = 0.5;
        a = 1.0;
    }
    if (col == "blue")
    {
        r = 0.0;
        g = 0.0;
        b = 1.0;
        a = 1.0;
    }
    if (col == "green")
    {
        r = 0.0;
        g = 1.0;
        b = 0.0;
        a = 1.0;
    }
    glColor4f(r, g, b, a);
    glLineWidth(5);
    glEnable(GL_LINE_SMOOTH);
    glBegin(GL_LINES);
    glVertex2i(xa, ya);
    glVertex2i(xb, yb);
    glEnd();
}