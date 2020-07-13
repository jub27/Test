using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Group : MonoBehaviour
{
    float lastFall = 0;

    private void Start()
    {
        if(!isValidGridPos())
        {
            Debug.Log("GAME OVER");
            Destroy(gameObject);
        }
    }

    //GroupObject내부에 있는 자식들의 위치를 알아낸다.
    bool isValidGridPos()
    {
        foreach (var child in transform)
        {
            //위치를 보정해준다
            Vector2 v = PlayField.roundVec2((child as Transform).position);

            //Board 안에 있는지 확인한다.
            if (!PlayField.insideBorder(v))
                return false;

            //Grid에 이미 블록이 존재하는지 체크한다.
            if (PlayField.grid[(int)v.x, (int)v.y] != null && PlayField.grid[(int)v.x, (int)v.y].parent != transform)
                return false;
        }

        return true;
    }

    //위치가 변경될때 마다 Grid에서 블록의 위치를 갱신한다.
    void updateGrid()
    {
        //위치가 변경될때에 이전 Grid위치에 있던 블록정보를 제거한다.
        for(int y = 0; y < PlayField.h; ++y)
        {
            for(int x = 0; x < PlayField.w; ++x)
            {
                if(PlayField.grid[x, y] != null)
                {
                    if (PlayField.grid[x, y] != null)
                    {
                        if (PlayField.grid[x, y].parent == transform)
                            PlayField.grid[x, y] = null;
                    }
                }
            }
        }

        //새로 추가된 블록정보를 추가한다.
        foreach(Transform child in transform)
        {
            Vector2 v = PlayField.roundVec2(child.position);
            PlayField.grid[(int)v.x, (int)v.y] = child;
        }
    }

    private void Update()
    {
        // 왼쪽으로 이동
        if (Input.GetKeyDown(KeyCode.LeftArrow))
        {
            // 왼쪽으로 한 칸 이동
            transform.position += new Vector3(-1, 0, 0);

            // 움직일 수 있는 위치라면
            if (isValidGridPos())
                // Grid 정보를 갱신하고 위치를 갱신한다.
                updateGrid();
            else
                // 그렇지 않다면 위치값을 보정한다.
                transform.position += new Vector3(1, 0, 0);
        }
        // 오른쪽으로 이동
        else if (Input.GetKeyDown(KeyCode.RightArrow))
        {
            // 오른쪽으로 한 칸 이동
            transform.position += new Vector3(1, 0, 0);

            // 움직일 수 있는 위치라면
            if (isValidGridPos())
                // Grid 정보를 갱신하고 위치를 갱신한다.
                updateGrid();
            else
                // 그렇지 않다면 위치값을 보정한다.
                transform.position += new Vector3(-1, 0, 0);
        }
        // 회전
        else if (Input.GetKeyDown(KeyCode.UpArrow))
        {
            //90도 회전한다.
            transform.Rotate(0, 0, -90);

            // 움직일 수 있는 위치라면
            if (isValidGridPos())
                // Grid 정보를 갱신하고 위치를 갱신한다.
                updateGrid();
            else
                // 그렇지 않다면 위치값을 보정한다.
                transform.Rotate(0, 0, 90);
        }
        // 아래로 움직인다, 마지막 줄까지
        else if (Input.GetKeyDown(KeyCode.DownArrow) ||
                 Time.time - lastFall >= 1)
        {
            // 아래로 한 칸 이동
            transform.position += new Vector3(0, -1, 0);

            // 움직일 수 있는 위치라면
            if (isValidGridPos())
            {
                // Grid 정보를 갱신하고 위치를 갱신한다.
                updateGrid();
            }
            else
            {
                // 그렇지 않다면 위치값을 보정한다.
                transform.position += new Vector3(0, 1, 0);

                // Grid에 라인이 다 찼는지 확인하고 삭제한다.
                PlayField.deleteFullRows();

                // 다음 블록을 생성한다.
                FindObjectOfType<Spawner>().spawnNext();

                // Group Script를 비활성화해서 더이상 컨트롤러의 적용을 받지 않도록 한다.
                enabled = false;
            }

            lastFall = Time.time;
        }
    }
}
