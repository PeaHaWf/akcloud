import { Box, Typography } from '@mui/material'
import File from '../components/File'
import AddFile from '@renderer/components/AddFile'
import { useEffect, useState } from 'react'
import { FileProps } from '@renderer/type'

const Files = () => {
  const [files, setFiles] = useState<FileProps[]>([])

  useEffect(() => {
    const fetchData = async () => {
      console.log('fetchData called')
      try {
        console.log('Sending request to http://localhost:3001/api/files')
        const response = await fetch('http://localhost:3001/api/files')
        if (!response.ok) {
          throw new Error('Network response was not ok')
        }
        const data = await response.json()
        console.log('Request successful', data) // 请求成功日志
        setFiles(data)
        console.log('Files set', data) // 数据设置日志
      } catch (e) {
        console.log('Error fetching files:', e) // 错误日志
      }
    }

    fetchData()
  }, [])

  return (
    <Box sx={{ m: 2.5, width: '100%' }}>
      <Box sx={{ display: 'flex' }}>
        <Typography variant="h5" fontWeight="bold" sx={{ pb: 3 }}>
          文件
        </Typography>
        <Box sx={{ flexGrow: 1 }} />
        <AddFile />
      </Box>
      <Box sx={{ display: 'flex', flexWrap: 'wrap', gap: 2 }}>
        {files.map((file, index) => (
          <File key={index} {...file} />
        ))}
      </Box>
    </Box>
  )
}
export default Files
