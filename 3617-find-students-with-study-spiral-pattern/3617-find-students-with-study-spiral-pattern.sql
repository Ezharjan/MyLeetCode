# Write your MySQL query statement below
WITH RankedSessions AS (
    SELECT 
        student_id, 
        subject, 
        session_date, 
        hours_studied,
        ROW_NUMBER() OVER (PARTITION BY student_id ORDER BY session_date) as rn,
        LAG(session_date) OVER (PARTITION BY student_id ORDER BY session_date) as prev_date
    FROM study_sessions
),
StudentMetrics AS (
    SELECT 
        student_id,
        COUNT(DISTINCT subject) as cycle_length,
        COUNT(*) as total_sessions,
        MAX(DATEDIFF(session_date, prev_date)) as max_gap,
        SUM(hours_studied) as total_study_hours
    FROM RankedSessions
    GROUP BY student_id
),
BrokenPatterns AS (
    SELECT r1.student_id
    FROM RankedSessions r1
    JOIN StudentMetrics sm ON r1.student_id = sm.student_id
    JOIN RankedSessions r2 ON r1.student_id = r2.student_id AND r1.rn = r2.rn + sm.cycle_length
    WHERE r1.subject != r2.subject
)

SELECT 
    s.student_id, 
    s.student_name, 
    s.major, 
    sm.cycle_length, 
    sm.total_study_hours
FROM StudentMetrics sm
JOIN students s ON sm.student_id = s.student_id
WHERE sm.cycle_length >= 3
  AND sm.total_sessions >= 2 * sm.cycle_length
  AND sm.total_sessions % sm.cycle_length = 0
  AND (sm.max_gap <= 2 OR sm.max_gap IS NULL)
  AND sm.student_id NOT IN (SELECT student_id FROM BrokenPatterns)
ORDER BY sm.cycle_length DESC, sm.total_study_hours DESC;