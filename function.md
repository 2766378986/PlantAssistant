+ 1.植物信息管理
   - 添加,编辑,删除植物信息(包括名称,种类,照片,浇水频率)
   - 搜索和筛选植物列表
+ 2.养护提醒
   - 基于用户设定的浇水,施肥,修剪的频率进行自动提醒
   - 自定义提醒设置
+ 3.生长日志
   - 为每株植物添加生长记录,如高度变化,开花时间,叶子颜色变化等
   - 支持文字和图片形式的日志记录
+ 4.数据统计
   - 统计养护行为,如每月浇水次数,施肥频率等
   - 数据可视化展示,如图表显示植物养护历史
+ 5.生长预测和健康评分
   - 根据历史数据和输入条件,预判植物的生长情况并给出健康评分
   - 提供优化建议,例如增加过找时间或者更换土壤
+ 6.个性化养护计划
   - 根据用户输入的日程安排生成个性化的植物养护计划
   - 计划包含具体的任务列表和提醒,以便用户轻松管理多个植物的养护
+ 7.植物生长报告和统计
   - 生成植物的生长报告，记录每个植物的生长情况，并对养护任务（浇水、施肥、病虫害处理等）进行统计和分析。

# 数据库设计
## 数据库名称: PlantAssistant
### 1. 表: Plants (植物信息)
+ PlantID (主键): 唯一标识每株植物的ID (INT, 自增)
+ Name: 植物名称 (VARCHAR(100))
+ Species: 植物种类 (VARCHAR(100))
+ PhotoURL: 植物照片的URL或文件路径 (VARCHAR(255))
+ WateringFrequency: 浇水频率 (INT, 单位: 天)
+ FertilizingFrequency: 施肥频率 (INT, 单位: 天)
+ PruningFrequency: 修剪频率 (INT, 单位: 天)
+ LightRequirement: 光照需求 (VARCHAR(50))
+ TemperatureRequirement: 温度需求 (VARCHAR(50))
+ HumidityRequirement: 湿度需求 (VARCHAR(50))
+ SoilType: 土壤类型 (VARCHAR(50))
+ DateAdded: 添加日期 (DATETIME)
+ LastWatered: 上次浇水日期 (DATETIME)
+ LastFertilized: 上次施肥日期 (DATETIME)
+ LastPruned: 上次修剪日期 (DATETIME)
+ UserName (外键): 关联的用户ID (VARCHAR(20))

### 2. 表: Users (用户信息)
+ UserName: 用户名 (VARCHAR(20))
+ PasswordHash: 密码哈希值 (VARCHAR(255))
+ CreatedAt: 用户创建时间 (DATETIME)

### 3. 表: CareReminders (养护提醒)
+ ReminderID (主键): 唯一标识每个提醒的ID (INT, 自增)
+ PlantID (外键): 关联的植物ID (INT)
+ ReminderType: 提醒类型 (ENUM: 'Watering', 'Fertilizing', 'Pruning')
+ ReminderDate: 提醒日期 (DATETIME)
+ IsCompleted: 是否已完成 (BOOLEAN, 默认: FALSE)
+ UserName (外键): 关联的用户ID (VARCHAR(20))

### 4. 表: GrowthLogs (生长日志)
+ LogID (主键): 唯一标识每个日志的ID (INT, 自增)
+ PlantID (外键): 关联的植物ID (INT)
+ LogDate: 日志日期 (DATETIME)
+ Height: 植物高度 (FLOAT, 单位: 厘米)
+ FloweringStatus: 开花状态 (VARCHAR(50))
+ LeafColor: 叶子颜色 (VARCHAR(50))
+ Notes: 备注 (TEXT)
+ PhotoURL: 日志照片的URL或文件路径 (VARCHAR(255))
+ UserName (外键): 关联的用户ID (VARCHAR(20))

### 5. 表: CareStatistics (养护统计)
+ StatisticID (主键): 唯一标识每条统计数据的ID (INT, 自增)
+ PlantID (外键): 关联的植物ID (INT)
+ Month: 统计月份 (DATE)
+ WateringCount: 浇水次数 (INT)
+ FertilizingCount: 施肥次数 (INT)
+ PruningCount: 修剪次数 (INT)
+ UserName (外键): 关联的用户ID (VARCHAR(20))

### 6. 表: HealthScores (健康评分)
+ HealthScoreID (主键): 唯一标识每个健康评分的ID (INT, 自增)
+ PlantID (外键): 关联的植物ID (INT)
+ ScoreDate: 评分日期 (DATETIME)
+ HealthScore: 健康评分 (INT, 范围: 0-100)
+ GrowthPrediction: 生长预测 (TEXT)
+ OptimizationSuggestions: 优化建议 (TEXT)
+ UserName (外键): 关联的用户ID (VARCHAR(20))

### 7. 表: CarePlans (个性化养护计划)
+ PlanID (主键): 唯一标识每个计划的ID (INT, 自增)
+ UserName (外键): 关联的用户ID (VARCHAR(20))
+ PlantID (外键): 关联的植物ID (INT)
+ PlanDate: 计划日期 (DATETIME)
+ Tasks: 任务列表 (TEXT)
+ IsCompleted: 是否已完成 (BOOLEAN, 默认: FALSE)

### 8. 表: GrowthReports (植物生长报告)
+ ReportID (主键): 唯一标识每个报告的ID (INT, 自增)
+ PlantID (外键): 关联的植物ID (INT)
+ ReportDate: 报告日期 (DATETIME)
+ GrowthSummary: 生长总结 (TEXT)
+ CareTaskStatistics: 养护任务统计 (TEXT)
+ UserName (外键): 关联的用户ID (VARCHAR(20))



## 功能实现
### taskWidget
显示植物ID，植物名，操作，目标时间，完成状态，编辑和删除按钮