import { Box, Button } from '@mui/material'
import Add from '@mui/icons-material/Add'
const AddFile = () => {
  return (
    <Button
      variant="contained"
      startIcon={<Add />}
      size="small"
      sx={{
        padding: '0px 5px !important',
        height: '40px !important'
      }}
    >
      添加文件
    </Button>
  )
}
export default AddFile
