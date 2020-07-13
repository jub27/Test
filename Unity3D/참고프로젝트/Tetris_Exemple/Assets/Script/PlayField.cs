using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//GameManager 역활
public class PlayField : MonoBehaviour
{
    //하나의 타일을 1로 했을때의 크기, 테투리 안쪽에 위치할 수 있게 제한.
    public static int w = 10;
    public static int h = 20;

    //2차원 배열의 크기
    public static Transform[,] grid = new Transform[w, h];

    //회전으로 인한 정수값을 보전하기 위해 보정작업을 해준다.
    public static Vector2 roundVec2(Vector2 v)
    {
        //Vector2(1.02934, 2.3498) => Vector2(1, 2)로 보정해준다.
        return new Vector2(Mathf.Round(v.x), Mathf.Round(v.y));
    }

    //위치값이 경계선 안쪽에 있는지 체크한다.
    public static bool insideBorder(Vector2 pos)
    {
        return (pos.x >= 0 && pos.x < w && pos.y >= 0);
    }

    //특정행의 모든 블록을 삭제한다, 모든 행의 Grid가 채워졌을때.
    public static void deleteRow(int y)
    {
        for(int x = 0; x < w; ++x)
        {
            Destroy(grid[x, y].gameObject);
            grid[x, y] = null;
        }
    }

    //행이 삭제 될때마다 Grid가 채워진 행은 한칸씩 내려준다.
    public static void decreaseRow(int y)
    {
        for(int x = 0; x < w; ++x)
        {
            if(grid[x, y] != null)
            {
                //아래로 이동한다, Grid의 상태만 적용된 것이지 위치값이 변한것이 아니다.
                grid[x, y - 1] = grid[x, y];
                grid[x, y] = null;

                //위 치를 Update한다.
                grid[x, y - 1].position += new Vector3(0, -1, 0);
            }
        }
    }

    //특정 인덱스 위의 존재하는 모든행에게 적용한다.
    public static void decreaseRowsAbove(int y)
    {
        for (int i = y; i < h; ++i)
            decreaseRow(i);
    }

    //모든 열이 Grid로 채워졌는지 판별할 수 있다.
    public static bool isRowFull(int y)
    {
        for(int x = 0; x < w; ++x)
        {
            if (grid[x, y] == null)
                return false;
        }

        return true;
    }

    //모든 전체 행 을 삭제 하고 항상 위 행의 y 좌표를 1 씩 줄여준다.
    public static void deleteFullRows()
    {
        for(int y = 0; y < h; ++y)
        {
            if(isRowFull(y))
            {
                deleteRow(y);
                decreaseRowsAbove(y + 1);
                --y;
            }
        }
    }
}
